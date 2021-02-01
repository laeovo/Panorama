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

void quadratmalen(SDL_Renderer* renderer, int x, int y) {
    for (size_t i = 0; i < 20; ++i) {
        for (size_t j = 0; j < 20; ++j) {
            SDL_RenderDrawPoint(renderer, x+i, y+j);
        }
    }
}

int main(int argc, const char * argv[]) {
    srand(0);
    
    vector<string> bildpunkte{};
    bildpunkte.push_back("Obere linke Ecke von 'DHL'"); // 0
    bildpunkte.push_back("Obere linke Ecke vom U-Bahn-Schild-U"); // 1
    bildpunkte.push_back("Erste Dachspitze"); // 2
    bildpunkte.push_back("Untere linke Ecke vom vorderen Erker"); // 3
    bildpunkte.push_back("Dachspitze überm Schäfers"); // 4
    bildpunkte.push_back("Bank --> dicker Zaunpfahl rechts daneben --> erste vertikale Strebe links --> wo sie den Schnee berührt"); // 5
    bildpunkte.push_back("Baum links von der Bank --> oberer 060°-Zweig --> kleine schwarze Unterbrechung"); // 6
    bildpunkte.push_back("Die Mitte vom ö in Hörgeräte"); // 7
    bildpunkte.push_back("30. vertikaler Pfahl rechts vom zweiten dicken rechts von der Bank"); // 8
    bildpunkte.push_back("Unterer Rand von Abzweigung, siehe Bild02"); // 9
    
    vector<pair<int, int>> bild01{bildpunkte.size()};
    bild01[0] = {1867, 3107};
    bild01[1] = {500, 2888};
    bild01[2] = {1649, 1924};
    bild01[3] = {3571, 2968};
    bild01[4] = {3887, 1838};
    bild01[5] = {3170, 3716};
    bild01[6] = {1815, 190};
    
    vector<pair<int, int>> bild02{bildpunkte.size()};
    bild02[0] = {1046, 3095};
    bild02[1] = {0, 0};
    bild02[2] = {798, 1817};
    bild02[3] = {2688, 2965};
    bild02[4] = {2956, 1959};
    bild02[5] = {2345, 3665};
    bild02[6] = {0, 0};
    bild02[7] = {4071, 3057};
    bild02[8] = {4140, 3683};
    bild02[9] = {4089, 580};
    
    vector<pair<int, int>> bild03{bildpunkte.size()};
    bild03[0] = {0, 0};
    bild03[1] = {0, 0};
    bild03[2] = {0, 0};
    bild03[3] = {1144, 2934};
    bild03[4] = {1420, 1946};
    bild03[5] = {768, 3692};
    bild03[6] = {0, 0};
    bild03[7] = {2338, 3034};
    bild03[8] = {2387, 3551};
    bild03[9] = {2371, 971};
    
    pair<int, int> koordinatenBild01 = {0, 0};
    pair<int, int> koordinatenBild02 = {koordinatenBild01.first + bild01[0].first - bild02[0].first, koordinatenBild01.second + bild01[0].second - bild02[0].second};
    pair<int, int> koordinatenBild03 = {koordinatenBild02.first + bild02[3].first - bild03[3].first, koordinatenBild02.second + bild02[3].second - bild03[3].second};
    
    
    double scale = 0.2;
    
//    SDL_Init(SDL_INIT_VIDEO);
//
//    SDL_Window* win = SDL_CreateWindow("Panorama", 100, 100, 1200, 800, SDL_WINDOW_SHOWN);
//    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
//
//    // Hintergrund
//    SDL_RenderClear(renderer);
//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//    SDL_RenderClear(renderer);
//    SDL_RenderPresent(renderer);
//
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//
//    cout << "Koordinaten Bild 1: (" << koordinatenBild01.first << ", " << koordinatenBild01.second << ")" << endl;
//    cout << "Koordinaten Bild 2: (" << koordinatenBild02.first << ", " << koordinatenBild02.second << ")" << endl;
//    cout << "Koordinaten Bild 3: (" << koordinatenBild03.first << ", " << koordinatenBild03.second << ")" << endl;
//
//    for (size_t i = 0; i < bildpunkte.size(); ++i) {
//        cout << "Bild 1, Punkt " << i << ": (" << koordinatenBild01.first + bild01[i].first << ", " << koordinatenBild01.second + bild01[i].second << ")" << endl;
//        cout << "Bild 2, Punkt " << i << ": (" << koordinatenBild02.first + bild02[i].first << ", " << koordinatenBild02.second + bild02[i].second << ")" << endl;
//        cout << "Bild 3, Punkt " << i << ": (" << koordinatenBild03.first + bild03[i].first << ", " << koordinatenBild03.second + bild03[i].second << ")" << endl;
//        SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, 255);
//        quadratmalen(renderer, (koordinatenBild01.first + bild01[i].first)*scale, (koordinatenBild01.second + bild01[i].second)*scale);
//        quadratmalen(renderer, (koordinatenBild02.first + bild02[i].first)*scale, (koordinatenBild02.second + bild02[i].second)*scale);
//        quadratmalen(renderer, (koordinatenBild03.first + bild03[i].first)*scale, (koordinatenBild03.second + bild03[i].second)*scale);
//    }
//    SDL_RenderPresent(renderer);
//
//    SDL_UpdateWindowSurface(win);
//    bool quit = false;
//    while (!quit) {
//        SDL_Event e;
//        if (SDL_PollEvent(&e)) {
//            if (e.type == SDL_QUIT) {
//                quit = true;
//            }
//        }
//    }
//    SDL_DestroyRenderer(renderer);
//    SDL_DestroyWindow(win);
//    SDL_Quit();
//    cout << "SDL wurde 'beendet'" << endl;
    const MapSphaerischToKartesisch map{};
    
    const SphaerischesBild bild{cimg_library::CImg<unsigned char>("earth.jpg")};
    
    
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow("Panorama", 100, 100, 1200, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);

    // Hintergrund
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    for (int i = -600; i < 600; i += 20) {
        for (int j = -400; j < 400; j += 20) {
            cout << "i = " << i << ", j = " << j << endl;
            const KartesischeKoordinaten kart{i+0., j+0.};
            const SphaerischeKoordinaten spha{map.getUrbild(kart)};
            const Farbe farbe{bild.get(spha)};
            SDL_SetRenderDrawColor(renderer, farbe.r, farbe.g, farbe.b, 255);
            quadratmalen(renderer, kart.getX()+600, kart.getY()+400);
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
    
    cout << "Hello, World!\n";
    return 0;
}
