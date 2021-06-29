#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <vector>

#define cimg_display 0
#define cimg_use_jpeg

#include "CImg.h"
#include "SDL2/SDL.h"

#include "KartesischeKoordinaten.hpp"
#include "MapKartesischToScreen.hpp"
#include "MapSphaerischToKartesisch.hpp"
#include "Marker.hpp"
#include "MarkerTool.hpp"
#include "ScreenKoordinaten.hpp"
#include "SphaerischeKoordinaten.hpp"
#include "SphaerischesBild.hpp"
#include "Test.hpp"

using namespace std;

void quadratmalen(SDL_Renderer* renderer, const int x, const int y, const int aufloesung) {
    for (size_t i = 0; i < aufloesung; ++i) {
        for (size_t j = 0; j < aufloesung; ++j) {
            SDL_RenderDrawPoint(renderer, x+i, y+j);
        }
    }
}

void markerZeichnen(SDL_Renderer* renderer, const MapSphaerischToKartesisch& mapSphaToKart2D, const MapKartesischToScreen& mapKartToScreen, const vector<SphaerischesBild> bilder) {
    for (const SphaerischesBild& bild : bilder) {
        for (const MarkerSphaerisch& marker : bild.getAlleMarker()) {
            const ScreenKoordinaten screen{mapKartToScreen.get(mapSphaToKart2D.get(marker.getKoord()))};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            quadratmalen(renderer, screen.getX(), screen.getY(), 3);
        }
    }
}

void test1() {
    const int nrBilder{3};
    vector<SphaerischesBild> bilder{};
    vector<string> dateinamen{};
    const string verzeichnis{"/Users/leo/Code/Panorama/Tests/ruedigross/"};
    for (size_t i = 1; i <= nrBilder; ++i) {
        const string dateiname{"" + verzeichnis + to_string(i) + ".jpg"};
        dateinamen.push_back(dateiname);
        bilder.push_back({dateiname, 16, 24});
    }
    cout << "Bilder wurden erstellt." << endl;
    
    // Marker festlegen
    for (size_t i = 1; i <= nrBilder-1; ++i) {
        MarkerTool mt{750};
        const pair<KartesischeKoordinaten, KartesischeKoordinaten> erstesMarkerPaar{mt.markerBestimmen(dateinamen[i-1], dateinamen[i])};
        bilder[i-1].markerHinzufuegen({2*i+0, erstesMarkerPaar.first});
        bilder[i].markerHinzufuegen({2*i+0, erstesMarkerPaar.second});
        const pair<KartesischeKoordinaten, KartesischeKoordinaten> zweitesMarkerPaar{mt.markerBestimmen(dateinamen[i-1], dateinamen[i])};
        bilder[i-1].markerHinzufuegen({2*i+1, zweitesMarkerPaar.first});
        bilder[i].markerHinzufuegen({2*i+1, zweitesMarkerPaar.second});
        
    }
    // ausrichten
    for (size_t i = 1; i <= nrBilder-1; ++i) {
        bilder[i].ausrichten(bilder[i].getMarker(2*i+0)->getKoord(), bilder[i].getMarker(2*i+1)->getKoord(), bilder[i-1].getMarker(2*i+0)->getKoord(), bilder[i-1].getMarker(2*i+1)->getKoord());
    }
    
    // zeichnen
    const int groesse{1300};
    const MapSphaerischToKartesisch mapSphaToKart2D{0};
    const MapKartesischToScreen mapKart2DToScreen{groesse/2, groesse/2};
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Panorama", 0, 0, groesse, groesse, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    cout << "Rendere kartesische Ansicht aus sphärischem Bild" << endl;
    const int aufloesung{1};
    for (int i = 0; i < groesse; i += aufloesung) {
//        const chrono::high_resolution_clock::time_point t1{chrono::high_resolution_clock::now()};
//        const chrono::duration<double> dauer{t1-t0};
        for (int j = 0; j < groesse; j += aufloesung) {
            const ScreenKoordinaten screen{i, j};
            const KartesischeKoordinaten kart{mapKart2DToScreen.getUrbild(screen)};
            const SphaerischeKoordinaten spha{mapSphaToKart2D.getUrbild(kart)};
            Farbe farbe{0, 255, 0};
            size_t bildZaehler = 0;
            while (farbe.r == 0 && farbe.g == 255 && farbe.b == 0 && bildZaehler < bilder.size()) {
                farbe = bilder[bildZaehler].get(spha);
                ++bildZaehler;
            }
            SDL_SetRenderDrawColor(renderer, farbe.r, farbe.g, farbe.b, 255);
            quadratmalen(renderer, i, j, aufloesung);
        }
    }

    // Marker zeichnen
    markerZeichnen(renderer, mapSphaToKart2D, mapKart2DToScreen, bilder);
    
    SDL_RenderPresent(renderer);

    // Fenster am Leben halten
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
}
