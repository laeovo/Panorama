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
    cout << "Erstelle sphärisches Bild..." << endl;
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
    const double distMarker{marker1.angularDistance(marker2)};
    const double distRef{referenz1.angularDistance(referenz2)};
    if (abs(distMarker-distRef) > 0.001) {
        cout << "Warnung: Marker haben unterschiedliche Abstände!" << endl;
        cout << " Marker1 - Marker2 : " << distMarker << " = " << distMarker*180/M_PI << " °" << endl;
        cout << " Ref1 - Ref2       : " << distRef << " = " << distRef*180/M_PI << " °" << endl;
    }
    const chrono::high_resolution_clock::time_point t0{chrono::high_resolution_clock::now()};
    /// Ab hier: alter Code ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//    cout << "Ausrichten... (1/2)" << endl;
//    const Kartesische3DKoordinaten drehachse1{Kartesische3DKoordinaten(marker1), Kartesische3DKoordinaten(referenz1)};
//    const double winkel1{marker1.angularDistance(referenz1)};
//    this->allesUmAchseDrehen(drehachse1, winkel1);
//    this->zentrumVerschieben(referenz1);
//
//    cout << "Ausrichten... (2/2)" << endl;
//    const Kartesische3DKoordinaten drehachse2{referenz1};
//    const double alpha{winkelZwischenVektoren({referenz1}, {referenz2})};
//    const double abschnittAufDrehachse{cos(alpha)};
//    const Kartesische3DKoordinaten punktAufDrehachse{drehachse2.x*abschnittAufDrehachse, drehachse2.y*abschnittAufDrehachse, drehachse2.z*abschnittAufDrehachse};
//    const Kartesische3DKoordinaten richtungsvektorMarker2{Kartesische3DKoordinaten(marker2)-punktAufDrehachse};
//    const Kartesische3DKoordinaten richtungsvektorReferenz2{Kartesische3DKoordinaten(referenz2)-punktAufDrehachse};
//    const Kartesische3DKoordinaten drehachse2AusMarkern{richtungsvektorMarker2, richtungsvektorReferenz2};
//    double winkel2{winkelZwischenVektoren(richtungsvektorMarker2, richtungsvektorReferenz2)};
//    if (skalarprodukt(drehachse2, drehachse2AusMarkern) > 0) {
//        // gegen den UZS drehen
//    }
//    else {
//        // im UZS drehen
//        winkel2 *= -1;
//    }
//    this->allesUmAchseDrehen(drehachse2, winkel2);
    /// Bis hier: alter Code ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    const Kartesische3DKoordinaten drehachse{Kartesische3DKoordinaten(Kartesische3DKoordinaten(referenz1)-Kartesische3DKoordinaten(marker1), Kartesische3DKoordinaten(referenz2)-Kartesische3DKoordinaten(marker2)).normiert()};
    const double abschnittAufDrehachse{skalarprodukt(drehachse, marker1)};
    const Kartesische3DKoordinaten punktAufDrehachse{abschnittAufDrehachse, drehachse};
    const double winkel{winkelZwischenVektoren(Kartesische3DKoordinaten(marker1)-punktAufDrehachse, Kartesische3DKoordinaten(referenz1)-punktAufDrehachse)};
    this->allesUmAchseDrehen(drehachse, winkel);
    
    // TODO: überprüfen, ob Drehung geklappt hat
    
    const chrono::high_resolution_clock::time_point t1{chrono::high_resolution_clock::now()};
    const chrono::duration<double> dauer {t1-t0};
    cout << "Fertig (" << dauer.count() << " Sekunden)" << endl;
}

void SphaerischesBild::zentrumVerschieben(const SphaerischeKoordinaten& neuesZentrum) {
    this->mitte = neuesZentrum;
}

void SphaerischesBild::markerHinzufuegen(const MarkerKartesisch& neuerMarker) {
    assert(!this->wurdeTransformiert);
    this->markerHinzufuegen(MarkerSphaerisch(neuerMarker.getId(), this->mapKartToSpha.get(neuerMarker.getKoord())));
}

void SphaerischesBild::markerHinzufuegen(const MarkerSphaerisch& neuerMarker) {
    this->marker.push_back(neuerMarker);
}

const MarkerSphaerisch* const SphaerischesBild::getMarker(const size_t id) const {
    for (const MarkerSphaerisch& marker : this->marker) {
        if (marker.getId() == id) {
            return &marker;
        }
    }
    cout << "Kein Marker mit der ID '" << id << "' gefunden :/" << endl;
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
