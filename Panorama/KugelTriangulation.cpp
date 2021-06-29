#include <cmath>
#include <iostream>
#include <random>

#include "SDL2/SDL.h"

#include "KugelTriangulation.hpp"

using namespace std;

KugelTriangulation::KugelTriangulation(const double aufloesung) : aufloesung(aufloesung) {
    const chrono::high_resolution_clock::time_point t0{chrono::high_resolution_clock::now()};
    for (double phi = M_PI/2-this->aufloesung; phi > -M_PI/2; phi -= this->aufloesung) {
        const double umfang{2*M_PI*cos(phi)};
        const int anzahl{int(umfang/aufloesung+1)};
        const double breite{2*M_PI/anzahl};
        this->zahlen.push_back(anzahl);
        this->breiten.push_back(breite);
    }
    const chrono::high_resolution_clock::time_point t1{chrono::high_resolution_clock::now()};
    const chrono::duration<double> dauer{t1-t0};
    cout << "Triangulation erstellt: " << this->zahlen.size() << " Zeilen mit " << this->getAnzahlRegionen() << " Regionen. Auflösung: " << this->aufloesung << " (" << dauer.count() << " Sekunden)" << endl;
}

const unsigned int KugelTriangulation::getRegion(const SphaerischeKoordinaten& koord) const {
    unsigned int output{0};
    unsigned int counter{0};
    for (double lat = M_PI/2-this->aufloesung; lat > -M_PI/2; lat -= this->aufloesung) {
        if (lat > koord.getLat()) {
            output += zahlen[counter];
            ++counter;
        }
        else {
            return output + int((M_PI+koord.getLon())/this->breiten[counter]);
        }
    }
    return 0;
}

const unsigned int KugelTriangulation::getAnzahlRegionen() const {
    unsigned int output{0};
    for (const unsigned int anzahl : this->zahlen) {
        output += anzahl;
    }
    return output;
}

void KugelTriangulation::zeichnen() const {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Kugel Triangulation", 0, 0, 1000, 500, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    srand(time(NULL));
    cout << "Zeichne Kugel-Triangulation" << endl;
    for (size_t i = 0; i < this->breiten.size(); ++i) {
        const double lat{M_PI/2 - this->aufloesung*(i+1)};
        const double breite{this->breiten[i]};
        const double hoehe{this->aufloesung};
        for (size_t j = 0; j < this->zahlen[i]; ++j) {
            const double lon{-M_PI+this->breiten[i]*j};
            // Random color
            SDL_SetRenderDrawColor(renderer, rand()%256, rand()%256, rand()%256, 255);
            for (int x = floor((lon+M_PI)*1000/(2*M_PI)); x < floor((lon+breite+M_PI)*1000/(2*M_PI)); ++x) {
                for (int y = floor((-lat+M_PI/2-hoehe)*500/(M_PI)); y < floor((-lat+M_PI/2)*500/(M_PI)); ++y) {
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
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
}
