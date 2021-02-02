#include <iostream>

#include "SphaerischesBild.hpp"

using namespace std;

SphaerischesBild::SphaerischesBild(cimg_library::CImg<unsigned char> bild) {
    this->bild = {this->triangulation.getAnzahlRegionen(), {{}}};
    const double aufloesungLon{2*M_PI / bild.width()};
    const double aufloesungLat{M_PI / bild.height()};
    unsigned int pixelcounter{0};
    for (int i = 0; i < bild.width(); ++i) {
        for (int j = 0; j < bild.height(); ++j) {
            const SphaerischeKoordinaten koord{M_PI - i*aufloesungLon, -M_PI/2 + (bild.height()-j-1)*aufloesungLat};
            const Farbe farbe{bild(i, j, 0, 0), bild(i, j, 0, 1), bild(i, j, 0, 2)};
            this->bild[this->triangulation.getRegion(koord)].push_back({koord, farbe});
            ++pixelcounter;
        }
    }
    cout << "Sphärisches Bild erstellt: " << pixelcounter << " Pixel in " << this->triangulation.getAnzahlRegionen() << " Regionen" << endl;
}

SphaerischesBild::SphaerischesBild(cimg_library::CImg<unsigned char> bild, const double brennweite, const double sensorbreite) {
    this->bild = {this->triangulation.getAnzahlRegionen(), {{}}};
    const double seitenverhaeltnis{(0.+bild.height())/bild.width()};
    const double gesichtsfeldHorizontal{2*atan(0.5*sensorbreite/brennweite)};
    const double gesichtsfeldVertikal{2*atan(0.5*(sensorbreite*seitenverhaeltnis)/brennweite)};
    const double gesichtsfeldDiagonal{2*atan(0.5*(sensorbreite*sqrt(1+pow(seitenverhaeltnis,2)))/brennweite)};
    const double diagonalenlaenge{2*sin(0.5*gesichtsfeldDiagonal)};
    const double projektionsebene{sqrt(1-pow(diagonalenlaenge/2,2))};
    const double eckeObenLinksY{sqrt((1-pow(projektionsebene,2))/(pow(seitenverhaeltnis,2)+1))};
    const double eckeObenLinksZ{eckeObenLinksY*seitenverhaeltnis};
    const double eckeUntenRechtsY{-eckeObenLinksY};
    const double eckeUntenRechtsZ{-eckeObenLinksZ};
    
    unsigned int pixelcounter{0};
    for (int i = 0; i < bild.width(); ++i) {
        for (int j = 0; j < bild.height(); ++j) {
            // Kartesische Koordinaten berechnen
            const double x{projektionsebene};
            const double y{eckeObenLinksY+((0.+i)/bild.width())*(eckeUntenRechtsY-eckeObenLinksY)};
            const double z{eckeObenLinksZ+((0.+j)/bild.height())*(eckeUntenRechtsZ-eckeObenLinksZ)};
            
            // projizierte Koordinaten berechnen
            const double faktor{1/sqrt(pow(x,2)+pow(y,2)+pow(z,2))};
            const double xProjiziert{x*faktor};
            const double yProjiziert{y*faktor};
            const double zProjiziert{z*faktor};
            
            // spährische Koordinaten berechnen
            const double lambda{atan2(yProjiziert, xProjiziert)};
            const double phi{asin(zProjiziert)};
            
            const SphaerischeKoordinaten koord{lambda,phi};
            const Farbe farbe{bild(i, j, 0, 0), bild(i, j, 0, 1), bild(i, j, 0, 2)};
            this->bild[this->triangulation.getRegion(koord)].push_back({koord, farbe});
            ++pixelcounter;
        }
    }
    cout << "Sphärisches Bild erstellt: " << pixelcounter << " Pixel in " << this->triangulation.getAnzahlRegionen() << " Regionen" << endl;
}

const Farbe SphaerischesBild::get(const SphaerischeKoordinaten& koordinaten) const {
    double gesamtgewicht{0};
    double r{0};
    double g{0};
    double b{0};
    for (const pair<SphaerischeKoordinaten, Farbe>& datenpaar : this->bild[this->triangulation.getRegion(koordinaten)]) {
        const SphaerischeKoordinaten& koord{datenpaar.first};
        const Farbe& farbe{datenpaar.second};
        const double entfernung{koordinaten.angularDistance(koord)};
        if (entfernung < this->schaerfe) {
            const double gewicht{this->schaerfe - entfernung};
            assert(gewicht >= 0);
            r += gewicht * farbe.r;
            g += gewicht * farbe.g;
            b += gewicht * farbe.b;
            gesamtgewicht += this->schaerfe-entfernung;
        }
    }
    r /= gesamtgewicht;
    g /= gesamtgewicht;
    b /= gesamtgewicht;
    if (gesamtgewicht == 0) {
//        cout << "Warnung: keine Pixel gefunden!" << endl;
        return {255,255,255};
    }
    return {(unsigned char)r, (unsigned char)g, (unsigned char)b};
}
