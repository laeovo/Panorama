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
#include "ScreenKoordinaten.hpp"
#include "SphaerischeKoordinaten.hpp"
#include "SphaerischesBild.hpp"

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

int main(int argc, const char * argv[]) {
    srand(0);
    const chrono::high_resolution_clock::time_point t0{chrono::high_resolution_clock::now()};
    
    const size_t anzahlBilder{7};
    vector<SphaerischesBild> bilder{};
    for (size_t i = 1; i <= anzahlBilder; ++i) {
        string dateiname{"/Users/leo/Pictures/2021-01-30 Schnee/Panorama/"};
        dateiname.append(to_string(i));
        dateiname.append(".jpg");
        SphaerischesBild bildSphaere{dateiname, 16, 24};
//        if (i > 1) {
//            bildSphaere.pan(-2*M_PI/12*(i-1));
//        }
//        bildSphaere.drehenImUZS(-0.05);
        bilder.push_back(bildSphaere);
    }
    
    vector<vector<pair<MarkerKartesisch, MarkerKartesisch>>> kartesischeMarker(anzahlBilder);
    kartesischeMarker[0].push_back({MarkerKartesisch("Hausspitze", {1125, 532}), MarkerKartesisch("30. Strebe", {1165, 1092})});
    kartesischeMarker[1].push_back({MarkerKartesisch("Hausspitze", {852, 572}), MarkerKartesisch("30. Strebe", {882, 1062})});
    
    kartesischeMarker[1].push_back({MarkerKartesisch("Dachecke", {1145, 697}), MarkerKartesisch("Strebe mit schraegem Zweig", {1152, 1066})});
    kartesischeMarker[2].push_back({MarkerKartesisch("Dachecke", {653, 722}), MarkerKartesisch("Strebe mit schraegem Zweig", {659, 1028})});

    kartesischeMarker[2].push_back({MarkerKartesisch("Balkonecke", {1186, 565}), MarkerKartesisch("Handlaufkurve", {1138, 926})});
    kartesischeMarker[3].push_back({MarkerKartesisch("Balkonecke", {702, 618}), MarkerKartesisch("Handlaufkurve", {670, 913})});
    
    kartesischeMarker[3].push_back({MarkerKartesisch("L", {1070, 549}), MarkerKartesisch("Gelaender Meets Baum", {1162, 932})});
    kartesischeMarker[4].push_back({MarkerKartesisch("L", {594, 588}), MarkerKartesisch("Gelaender Meets Baum", {660, 917})});
    
    kartesischeMarker[4].push_back({MarkerKartesisch("DachgiebelMeetsBaum", {1100, 588}), MarkerKartesisch("MauerMeetsBaum", {1112, 939})});
    kartesischeMarker[5].push_back({MarkerKartesisch("DachgiebelMeetsBaum", {628, 626}), MarkerKartesisch("MauerMeetsBaum", {638, 925})});
    
    kartesischeMarker[5].push_back({MarkerKartesisch("Baumdelle", {1143, 600}), MarkerKartesisch("Schneefleck", {1149, 904})});
    kartesischeMarker[6].push_back({MarkerKartesisch("Baumdelle", {661, 641}), MarkerKartesisch("Schneefleck", {665, 895})});
    
    for (size_t i = 0; i < kartesischeMarker.size(); ++i) {
        for (const pair<MarkerKartesisch, MarkerKartesisch>& markerpaar : kartesischeMarker[i]) {
            bilder[i].markerHinzufuegen(markerpaar.first);
            bilder[i].markerHinzufuegen(markerpaar.second);
        }
    }
    
    for (size_t i = 1; i < kartesischeMarker.size(); ++i) {
        bilder[i].ausrichten(bilder[i].getMarker(kartesischeMarker[i][0].first.getName())->getKoord(), bilder[i].getMarker(kartesischeMarker[i][0].second.getName())->getKoord(), bilder[i-1].getMarker(kartesischeMarker[i][0].first.getName())->getKoord(), bilder[i-1].getMarker(kartesischeMarker[i][0].second.getName())->getKoord());
    }

    const int groesse{1024};
    const MapSphaerischToKartesisch mapSphaToKart2D{3*M_PI/4};
    const MapKartesischToScreen mapKart2DToScreen{groesse/2, groesse/2};
    
    // Hintergrund
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
        const chrono::high_resolution_clock::time_point t1{chrono::high_resolution_clock::now()};
        const chrono::duration<double> dauer{t1-t0};
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
    
    markerZeichnen(renderer, mapSphaToKart2D, mapKart2DToScreen, bilder);
    
    const chrono::high_resolution_clock::time_point t1{chrono::high_resolution_clock::now()};
    const chrono::duration<double> dauer{t1-t0};
    cout << dauer.count() << " Sekunden" << endl;
    

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
    return 0;
}
