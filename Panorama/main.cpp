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

int main(int argc, const char * argv[]) {
    
    const KartesischeKoordinaten marker1_bild1{1125, 532}; // Hausspitze
    const KartesischeKoordinaten marker1_bild2{852, 572};
    const KartesischeKoordinaten marker2_bild1{1165, 1092}; // 30. vertikale Strebe
    const KartesischeKoordinaten marker2_bild2{882, 1062};
    const KartesischeKoordinaten marker3_bild2{1145, 697}; // Untere rechte Ecke des Schrägdachs
    const KartesischeKoordinaten marker3_bild3{653, 722};
    const KartesischeKoordinaten marker4_bild2{1152, 1066}; // Vertikale Strebe mit schrägem Zweig
    const KartesischeKoordinaten marker4_bild3{659, 1028};
    
    vector<SphaerischesBild> bilder{};
    for (size_t i = 1; i <= 3; ++i) {
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
    
    bilder[0].markerHinzufuegen(marker1_bild1, "Hausspitze");
    bilder[1].markerHinzufuegen(marker1_bild2, "Hausspitze");
    bilder[0].markerHinzufuegen(marker2_bild1, "30. Strebe");
    bilder[1].markerHinzufuegen(marker2_bild2, "30. Strebe");
    bilder[1].markerHinzufuegen(marker3_bild2, "Dachecke");
    bilder[2].markerHinzufuegen(marker3_bild3, "Dachecke");
    bilder[1].markerHinzufuegen(marker4_bild2, "Strebe mit schraegem Zweig");
    bilder[2].markerHinzufuegen(marker4_bild3, "Strebe mit schraegem Zweig");
    
    bilder[1].ausrichten(bilder[1].getMarker("Hausspitze")->first, bilder[1].getMarker("30. Strebe")->first, bilder[0].getMarker("Hausspitze")->first, bilder[0].getMarker("30. Strebe")->first);
    bilder[2].ausrichten(bilder[2].getMarker("Dachecke")->first, bilder[2].getMarker("Strebe mit schraegem Zweig")->first, bilder[1].getMarker("Dachecke")->first, bilder[1].getMarker("Strebe mit schraegem Zweig")->first);

    const int groesse{1024};
    const MapSphaerischToKartesisch mapSphaToKart2D{};
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
    const chrono::high_resolution_clock::time_point t0{chrono::high_resolution_clock::now()};
    const int aufloesung{1};
    for (int i = 0; i < groesse; i += aufloesung) {
        const chrono::high_resolution_clock::time_point t1{chrono::high_resolution_clock::now()};
        const chrono::duration<double> dauer{t1-t0};
        cout << "Geschafft: " << 100.*i/groesse << "% in " <<  dauer.count() << " Sekunden. Gesamtdauer: " << dauer.count()*groesse/(i+groesse) << " Sekunden" << endl;
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
    cout << "SDL wurde 'beendet'" << endl;
    
    cout << "Hello, World!\n";
    return 0;
}
