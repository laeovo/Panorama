#include <chrono>
#include <iostream>

#include "SDL2/SDL.h"

#include "Kartesische3DKoordinaten.hpp"
#include "SphaerischesBild.hpp"

using namespace std;

SphaerischesBild::SphaerischesBild(cimg_library::CImg<unsigned char> bild) {
    const chrono::high_resolution_clock::time_point t0{chrono::high_resolution_clock::now()};
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
    const chrono::high_resolution_clock::time_point t1{chrono::high_resolution_clock::now()};
    const chrono::duration<double> dauer{t1-t0};
    cout << "Sphärisches Bild erstellt: " << pixelcounter << " Pixel in " << this->triangulation.getAnzahlRegionen() << " Regionen (" << dauer.count() << " Sekunden)" << endl;
}

SphaerischesBild::SphaerischesBild(const string& dateiname, const double brennweite, const double sensorbreite) {
    // TODO: (Teile hiervon) nach MapKartesischToSphaerisch bewegen?
    const chrono::high_resolution_clock::time_point t0{chrono::high_resolution_clock::now()};
    cimg_library::CImg<unsigned char> bild(dateiname.c_str());
    this->bild = {this->triangulation.getAnzahlRegionen(), {{}}};
    const double seitenverhaeltnis{(0.+bild.height())/bild.width()};
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
    const chrono::high_resolution_clock::time_point t1{chrono::high_resolution_clock::now()};
    const chrono::duration<double> dauer{t1-t0};
    cout << "Sphärisches Bild erstellt: " << pixelcounter << " Pixel in " << this->triangulation.getAnzahlRegionen() << " Regionen (" << dauer.count() << " Sekunden)" << endl;
}

const Farbe SphaerischesBild::get(const SphaerischeKoordinaten& koordinaten) const {
    double gesamtgewicht{0};
    double r{0};
    double g{0};
    double b{0};
//    cout << "Region: " << this->triangulation.getRegion(koordinaten) << endl;
//    cout << "Anzahl Regionn: " << this->bild.size() << endl;
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
        return {0,255,0};
    }
    return {(unsigned char)r, (unsigned char)g, (unsigned char)b};
}

void SphaerischesBild::print() const {
    // Hintergrund
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Sphärische Ansicht", 100, 100, 1024, 512, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    cout << "Erzeuge sphärische Ansicht" << endl;
    for (int i = -512; i < 512; i += 1) {
        for (int j = -256; j < 256; j += 1) {
            const SphaerischeKoordinaten spha{-M_PI + 2*M_PI*((512.+i)/1024), M_PI/2 - M_PI*((256.+j)/512)};
            Farbe farbe{0, 255, 0};
            farbe = this->get(spha);
            SDL_SetRenderDrawColor(renderer, farbe.r, farbe.g, farbe.b, 255);
            SDL_RenderDrawPoint(renderer, i+512, j+256);
        }
    }
    SDL_RenderPresent(renderer);

    SDL_UpdateWindowSurface(win);
    bool quit = false;
    while (!quit) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    cout << "SDL wurde 'beendet'" << endl;
}

void SphaerischesBild::pan(const double winkel) {
    cout << "Pan... ";
    const chrono::high_resolution_clock::time_point t0{chrono::high_resolution_clock::now()};
    for (vector<pair<SphaerischeKoordinaten, Farbe>>& region : this->bild) {
        for (pair<SphaerischeKoordinaten, Farbe>& pixel : region) {
            pixel.first = {pixel.first.getLon() + winkel, pixel.first.getLat()};
        }
    }
    this->lon += winkel;
    this->pixelAufRegionenAufteilen();
    const chrono::high_resolution_clock::time_point t1{chrono::high_resolution_clock::now()};
    const chrono::duration<double> dauer {t1-t0};
    cout << "Fertig (" << dauer.count() << " Sekunden)" << endl;
}

void SphaerischesBild::tilt(const double winkel) {
    cout << "Tilt... ";
    const chrono::high_resolution_clock::time_point t0{chrono::high_resolution_clock::now()};
    for (vector<pair<SphaerischeKoordinaten, Farbe>>& region : this->bild) {
        for (pair<SphaerischeKoordinaten, Farbe>& pixel : region) {
            SphaerischeKoordinaten& spha{pixel.first};
            const Kartesische3DKoordinaten drehachse{sin(this->lon), -cos(this->lat), 0};
            const Kartesische3DKoordinaten alt{cos(spha.getLon())*cos(spha.getLat()), sin(spha.getLon())*cos(spha.getLat()), sin(spha.getLat())};
            const Kartesische3DKoordinaten neu{rotationGegenUZS(alt, drehachse, winkel)};
            spha = {atan2(neu.y, neu.x), asin(neu.z)};
        }
    }
    this->lon += winkel; // TODO: Nochmal überlegen
    this->pixelAufRegionenAufteilen();
    const chrono::high_resolution_clock::time_point t1{chrono::high_resolution_clock::now()};
    const chrono::duration<double> dauer {t1-t0};
    cout << "Fertig (" << dauer.count() << " Sekunden)" << endl;
}

void SphaerischesBild::drehenImUZS(const double winkel) {
    cout << "Drehen... ";
    const chrono::high_resolution_clock::time_point t0{chrono::high_resolution_clock::now()};
    for (vector<pair<SphaerischeKoordinaten, Farbe>>& region : this->bild) {
        for (pair<SphaerischeKoordinaten, Farbe>& pixel : region) {
            SphaerischeKoordinaten& spha{pixel.first};
            const Kartesische3DKoordinaten drehachse{-cos(this->lon)*cos(this->lat), -sin(this->lon)*cos(this->lat), -sin(this->lat)};
            const Kartesische3DKoordinaten alt{cos(spha.getLon())*cos(spha.getLat()), sin(spha.getLon())*cos(spha.getLat()), sin(spha.getLat())};
            const Kartesische3DKoordinaten neu{rotationGegenUZS(alt, drehachse, winkel)};
            spha = {atan2(neu.y, neu.x), asin(neu.z)};
        }
    }
    this->pixelAufRegionenAufteilen();
    const chrono::high_resolution_clock::time_point t1{chrono::high_resolution_clock::now()};
    const chrono::duration<double> dauer {t1-t0};
    cout << "Fertig (" << dauer.count() << " Sekunden)" << endl;
}

void SphaerischesBild::zentrumVerschieben(const SphaerischeKoordinaten& neuesZentrum) {
    this->lon = neuesZentrum.getLon();
    this->lat = neuesZentrum.getLat();
}

void SphaerischesBild::pixelAufRegionenAufteilen() {
    vector<pair<SphaerischeKoordinaten, Farbe>> pixelZwischenspeicher{};
    for (const vector<pair<SphaerischeKoordinaten, Farbe>>& region : this->bild) {
        for (const pair<SphaerischeKoordinaten, Farbe>& pixel : region) {
            pixelZwischenspeicher.push_back(pixel);
        }
    }
    this->bild = {this->triangulation.getAnzahlRegionen(), {{}}};
    for (const pair<SphaerischeKoordinaten, Farbe>& pixel : pixelZwischenspeicher) {
        this->bild[this->triangulation.getRegion(pixel.first)].push_back(pixel);
    }
}
