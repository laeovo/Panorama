#include <chrono>
#include <iostream>

#define cimg_display 0
#define cimg_use_jpeg

#include "CImg.h"
#include "SDL2/SDL.h"

#include "Kartesische3DKoordinaten.hpp"
#include "KartesischeKoordinaten.hpp"
#include "SphaerischesBild.hpp"

using namespace std;

SphaerischesBild::SphaerischesBild(const string& dateiname, const double brennweite, const double sensorbreite) : mapKartToSpha(cimg_library::CImg<unsigned char>(dateiname.c_str()).width(), cimg_library::CImg<unsigned char>(dateiname.c_str()).height(), brennweite, sensorbreite) {
    const chrono::high_resolution_clock::time_point t0{chrono::high_resolution_clock::now()};
    cimg_library::CImg<unsigned char> bild(dateiname.c_str());
    this->bild = {this->triangulation.getAnzahlRegionen(), {{}}};
    
    unsigned int pixelcounter{0};
    for (int i = 0; i < bild.width(); ++i) {
        for (int j = 0; j < bild.height(); ++j) {
            const SphaerischeKoordinaten koord{this->mapKartToSpha.get({i+0., j+0.})};
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

void SphaerischesBild::allesUmAchseDrehen(const Kartesische3DKoordinaten& drehachse, const double winkel) {
    for (vector<pair<SphaerischeKoordinaten, Farbe>>& region : this->bild) {
        for (pair<SphaerischeKoordinaten, Farbe>& pixel : region) {
            SphaerischeKoordinaten& spha{pixel.first};
            spha = rotationGegenUZS(spha, drehachse, winkel);
        }
    }
    for (MarkerSphaerisch& marker : this->marker) {
        marker.setKoord(rotationGegenUZS(marker.getKoord(), drehachse, winkel));
    }
    this->mitte = rotationGegenUZS(this->mitte, drehachse, winkel);
    this->pixelAufRegionenAufteilen();
    this->wurdeTransformiert = true;
}

void SphaerischesBild::ausrichten(const SphaerischeKoordinaten& marker1, const SphaerischeKoordinaten& marker2, const SphaerischeKoordinaten& referenz1, const SphaerischeKoordinaten& referenz2) {
    cout << "Ausrichten..." << endl;
    const chrono::high_resolution_clock::time_point t0{chrono::high_resolution_clock::now()};
    
    const Kartesische3DKoordinaten mittelebene1{Kartesische3DKoordinaten(referenz1) - Kartesische3DKoordinaten(marker1)};
    const Kartesische3DKoordinaten mittelebene2{Kartesische3DKoordinaten(referenz2) - Kartesische3DKoordinaten(marker2)};
    const Kartesische3DKoordinaten drehachse{Kartesische3DKoordinaten(mittelebene1, mittelebene2).normiert()};
    
    const double alpha1{winkelZwischenVektoren(drehachse, marker1)};
    const double abschnittAufDrehachse1{cos(alpha1)};
    const Kartesische3DKoordinaten punktAufDrehachse1{drehachse.x * abschnittAufDrehachse1, drehachse.y * abschnittAufDrehachse1, drehachse.z * abschnittAufDrehachse1};
    const Kartesische3DKoordinaten richtungsvektorMarker1{Kartesische3DKoordinaten(marker1) - punktAufDrehachse1};
    const Kartesische3DKoordinaten richtungsvektorReferenz1{Kartesische3DKoordinaten(referenz1) - punktAufDrehachse1};
    const double winkel1{winkelZwischenVektoren(richtungsvektorMarker1, richtungsvektorReferenz1)};
    const Kartesische3DKoordinaten drehachseAusMarkern1{richtungsvektorMarker1, richtungsvektorReferenz1};
    
    const double alpha2{winkelZwischenVektoren(drehachse, marker2)};
    const double abschnittAufDrehachse2{cos(alpha2)};
    const Kartesische3DKoordinaten punktAufDrehachse2{drehachse.x * abschnittAufDrehachse2, drehachse.y * abschnittAufDrehachse2, drehachse.z * abschnittAufDrehachse1};
    const Kartesische3DKoordinaten richtungsvektorMarker2{Kartesische3DKoordinaten(marker2) - punktAufDrehachse2};
    const Kartesische3DKoordinaten richtungsvektorReferenz2{Kartesische3DKoordinaten(referenz2) - punktAufDrehachse2};
    const double winkel2{winkelZwischenVektoren(richtungsvektorMarker2, richtungsvektorReferenz2)};
    const Kartesische3DKoordinaten drehachseAusMarkern2{richtungsvektorMarker2, richtungsvektorReferenz2};
    
    double winkel{(winkel1+winkel2)/2};
    
    if (skalarprodukt(drehachse, drehachseAusMarkern1) > 0) {
        // gegen den UZS drehen
    }
    else {
        // im UZS drehen
        winkel *= -1;
    }
    this->allesUmAchseDrehen(drehachse, winkel);
    
//    cout << "Abstand1: " << marker1.angularDistance(referenz1) << " --> " << SphaerischeKoordinaten(rotationGegenUZS(marker1, drehachse, winkel)).angularDistance(referenz1) << endl;
//    cout << "Abstand2: " << marker2.angularDistance(referenz2) << " --> " <<SphaerischeKoordinaten(rotationGegenUZS(marker2, drehachse, winkel)).angularDistance(referenz2) << endl;
//    cout << "Abstand Marker vs. Referenz: " << marker1.angularDistance(marker2) << " vs. " << referenz1.angularDistance(referenz2) << ", Differenz = " << referenz1.angularDistance(referenz2) - marker1.angularDistance(marker2) << endl;
    const chrono::high_resolution_clock::time_point t1{chrono::high_resolution_clock::now()};
    const chrono::duration<double> dauer {t1-t0};
    cout << "Fertig (" << dauer.count() << " Sekunden)" << endl;
    cout << endl;
}

void SphaerischesBild::zentrumVerschieben(const SphaerischeKoordinaten& neuesZentrum) {
    this->mitte = neuesZentrum;
}

void SphaerischesBild::markerHinzufuegen(const MarkerKartesisch& neuerMarker) {
    assert(!this->wurdeTransformiert);
    this->markerHinzufuegen(MarkerSphaerisch(neuerMarker.getName(), this->mapKartToSpha.get(neuerMarker.getKoord())));
}

void SphaerischesBild::markerHinzufuegen(const MarkerSphaerisch& neuerMarker) {
    this->marker.push_back(neuerMarker);
}

const MarkerSphaerisch* const SphaerischesBild::getMarker(const std::string& name) const {
    for (const MarkerSphaerisch& marker : this->marker) {
        if (marker.getName() == name) {
            return &marker;
        }
    }
    cout << "Kein Marker mit dem Namen '" << name << "' gefunden :/" << endl;
    return NULL;
}

const vector<MarkerSphaerisch>& SphaerischesBild::getAlleMarker() const {
    return this->marker;
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
