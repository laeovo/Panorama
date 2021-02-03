#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <vector>

#define cimg_display 0
#define cimg_use_jpeg

#include "CImg.h"
#include "SDL2/SDL.h"

#include "KartesischeKoordinaten.hpp"
#include "MapSphaerischToKartesisch.hpp"
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
    
    const KartesischeKoordinaten marker1_bild1{1123, 534}; // Hausspitze
    const KartesischeKoordinaten marker2_bild1{1165, 1093}; // 30. vertikale Strebe
    const KartesischeKoordinaten marker1_bild2{854, 569};
    const KartesischeKoordinaten marker2_bild2{887, 1064};
    
    vector<SphaerischesBild> bilder{};
    for (size_t i = 1; i <= 2; ++i) {
        string dateiname{"/Users/leo/Pictures/2021-01-30 Schnee/Panorama/"};
        dateiname.append(to_string(i));
        dateiname.append(".jpg");
        SphaerischesBild bildSphaere{dateiname, 16, 36};
//        if (i > 1) {
//            bildSphaere.pan(-2*M_PI/12*(i-1));
//        }
//        bildSphaere.drehenImUZS(-0.05);
        bilder.push_back(bildSphaere);
    }
    
    bilder[0].markerHinzufuegen(marker1_bild1, "Hausspitze");
    bilder[0].markerHinzufuegen(marker2_bild1, "30. Strebe");
    bilder[1].markerHinzufuegen(marker1_bild2, "Hausspitze");
    bilder[1].markerHinzufuegen(marker2_bild2, "30. Strebe");
    
    bilder[1].pan(bilder[0].getMarker("Hausspitze")->first.getLon() - bilder[0].getMarker("30. Strebe")->first.getLon());
    bilder[1].tilt(-bilder[0].getMarker("Hausspitze")->first.getLat() + bilder[0].getMarker("30. Strebe")->first.getLat());
    
//    SphaerischesBild deckel{"/Users/leo/Pictures/2021-01-30 Schnee/Panorama/26.jpg", 16, 36};
//    deckel.tilt(M_PI/2);
//    deckel.pan(M_PI/2);
//    bilder.push_back(deckel);

    const MapSphaerischToKartesisch map{};
    const int groesse{1024};
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
    for (int i = -groesse/2; i < groesse/2; i += aufloesung) {
        const chrono::high_resolution_clock::time_point t1{chrono::high_resolution_clock::now()};
        const chrono::duration<double> dauer{t1-t0};
        cout << "Geschafft: " << (100./groesse)*(i+groesse/2) << "% in " <<  dauer.count() << " Sekunden. Gesamtdauer: " << dauer.count()*groesse/(i+512) << " Sekunden" << endl;
        for (int j = -groesse/2; j < groesse/2; j += aufloesung) {
            const KartesischeKoordinaten kart{i+0., j+0.};
            const SphaerischeKoordinaten spha{map.getUrbild(kart)};
            Farbe farbe{0, 255, 0};
            size_t i = 0;
            while (farbe.r == 0 && farbe.g == 255 && farbe.b == 0 && i < bilder.size()) {
                farbe = bilder[i].get(spha);
                ++i;
            }
            SDL_SetRenderDrawColor(renderer, farbe.r, farbe.g, farbe.b, 255);
            quadratmalen(renderer, kart.getX()+groesse/2, kart.getY()+groesse/2, aufloesung);
        }
    }
    
    const SphaerischeKoordinaten& marker1_bild1_spha{bilder[0].getMarker("Hausspitze")->first};
    const SphaerischeKoordinaten& marker1_bild2_spha{bilder[1].getMarker("Hausspitze")->first};
    const SphaerischeKoordinaten& marker2_bild1_spha{bilder[0].getMarker("30. Strebe")->first};
    const SphaerischeKoordinaten& marker2_bild2_spha{bilder[1].getMarker("30. Strebe")->first};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    marker1_bild1_spha.print();
    cout << map.get(marker1_bild1_spha).getX() << " " << -map.get(marker1_bild1_spha).getY() << endl;
    quadratmalen(renderer, map.get(marker1_bild1_spha).getX(), -map.get(marker1_bild1_spha).getY(), 3);
    quadratmalen(renderer, map.get(marker1_bild2_spha).getX(), -map.get(marker1_bild2_spha).getY(), 3);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    quadratmalen(renderer, map.get(marker2_bild1_spha).getX(), -map.get(marker2_bild1_spha).getY(), 3);
    quadratmalen(renderer, map.get(marker2_bild2_spha).getX(), -map.get(marker2_bild2_spha).getY(), 3);
    SDL_RenderPresent(renderer);
    
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
