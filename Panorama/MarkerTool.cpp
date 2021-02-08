#include <iostream>
#include <string>

#define cimg_display 0
#define cimg_use_jpeg

#include "CImg.h"
#include "SDL2/SDL.h"

#include "MarkerTool.hpp"

using namespace std;

MarkerTool::MarkerTool(const int fensterhoehe) : fensterhoehe(fensterhoehe) {}

const pair<KartesischeKoordinaten, KartesischeKoordinaten> MarkerTool::markerBestimmen(const std::string& dateiname1, const std::string& dateiname2) const {
    const pair<cimg_library::CImg<unsigned char>, cimg_library::CImg<unsigned char>> bilder{dateiname1.c_str(), dateiname2.c_str()};
    pair<KartesischeKoordinaten, KartesischeKoordinaten> marker{{bilder.first.width()/2., bilder.first.height()/2.}, {bilder.second.width()/2., bilder.second.height()/2.}};
    
    const int fensterbreite{2*this->fensterhoehe};
    
    pair<double, double> faktor{(0.+this->fensterhoehe)/max(bilder.first.width(), bilder.first.height()), (0.+this->fensterhoehe)/max(bilder.second.width(), bilder.second.height())};
    
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Panorama", 0, 0, fensterbreite, this->fensterhoehe, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    
    // Hintergrund
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    
    this->bildZeichnen(LINKES_BILD, bilder.first, renderer, marker.first, faktor.first);
    this->bildZeichnen(RECHTES_BILD, bilder.second, renderer, marker.second, faktor.second);

    SDL_UpdateWindowSurface(window);
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
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return marker;
}

void MarkerTool::bildZeichnen(const BildSeite seite, const cimg_library::CImg<unsigned char>& bild, SDL_Renderer* renderer, const KartesischeKoordinaten& neuerMarker, const double neuerFaktor) const {
    const double bildbreite{neuerFaktor*bild.width()};
    const double bildhoehe{neuerFaktor*bild.height()};
    
    const pair<int, int> eckeObenLinks{this->fensterhoehe/2-bildbreite*neuerMarker.getX()/bild.width(), this->fensterhoehe/2-bildhoehe*neuerMarker.getY()/bild.height()};
    const pair<int, int> eckeUntenRechts{eckeObenLinks.first+bildbreite, eckeObenLinks.second+bildhoehe};
    
    cout << "Ecke oben links: (" << eckeObenLinks.first << ", " << eckeObenLinks.second << ")" << endl;
    cout << "Ecke unten rechte: (" << eckeUntenRechts.first << ", " << eckeUntenRechts.second << ")" << endl;
    
    for (int i = max(eckeObenLinks.first, 0); i < min(this->fensterhoehe, eckeUntenRechts.first); ++i) {
        for (int j = max(eckeObenLinks.second, 0); j < min(this->fensterhoehe, eckeUntenRechts.second); ++j) {
            const int x{int((i-eckeObenLinks.first)/neuerFaktor)};
            const int y{int((j-eckeObenLinks.second)/neuerFaktor)};
            SDL_SetRenderDrawColor(renderer, bild(x, y, 0, 0), bild(x, y, 0, 1), bild(x, y, 0, 2), 255);
            SDL_RenderDrawPoint(renderer, i+seite*this->fensterhoehe, j);
        }
    }
    // TODO: Fadenkreuz zeichnen
    // TODO: Button zeichnen
}
