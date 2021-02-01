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
    for (const vector<pair<SphaerischeKoordinaten, Farbe>>& region : this->bild) {
        if (region.size() == 0) {
            cout << "Warnung: Region mit 0 Einträgen" << endl;
        }
    }
}

SphaerischesBild::SphaerischesBild(cimg_library::CImg<unsigned char> bild, const double brennweite, const double sensorbreite) {
    this->bild = {this->triangulation.getAnzahlRegionen(), {{}}};
    const double gesichtsfeldHorizontal{2*atan(0.5*sensorbreite/brennweite)};
    const double gesichtsfeldVertikal{2*atan(0.5*(sensorbreite*((0.+bild.height())/bild.width()))/brennweite)};
    
    unsigned int pixelcounter{0};
    for (int i = 0; i < bild.width(); ++i) {
        for (int j = 0; j < bild.height(); ++j) {
            // Kartesische Koordinaten berechnen
            
            
            const SphaerischeKoordinaten koord{M_PI - i*aufloesungLon, -M_PI/2 + (bild.height()-j-1)*aufloesungLat};
            const Farbe farbe{bild(i, j, 0, 0), bild(i, j, 0, 1), bild(i, j, 0, 2)};
            this->bild[this->triangulation.getRegion(koord)].push_back({koord, farbe});
            ++pixelcounter;
        }
    }
    cout << "Sphärisches Bild erstellt: " << pixelcounter << " Pixel in " << this->triangulation.getAnzahlRegionen() << " Regionen" << endl;
    for (const vector<pair<SphaerischeKoordinaten, Farbe>>& region : this->bild) {
        if (region.size() == 0) {
            cout << "Warnung: Region mit 0 Einträgen" << endl;
        }
    }
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
        cout << "Warnung: keine Pixel gefunden!" << endl;
    }
    return {(unsigned char)r, (unsigned char)g, (unsigned char)b};
}
