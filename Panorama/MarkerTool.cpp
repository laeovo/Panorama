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
    SDL_Window* window = SDL_CreateWindow("Markertool", 0, 0, fensterbreite, this->fensterhoehe, SDL_WINDOW_SHOWN);
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
            else if (e.type == SDL_MOUSEBUTTONDOWN){
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= this->fensterhoehe-this->buttongroesse && x < this->fensterhoehe+this->buttongroesse+1 && y >= this->fensterhoehe-this->buttongroesse) {
                    return marker;
                }
                if (x < this->fensterhoehe) {
                    const double bildbreite{faktor.first*bilder.first.width()};
                    const double bildhoehe{faktor.first*bilder.first.height()};
                    const pair<int, int> eckeObenLinks{this->fensterhoehe/2-bildbreite*marker.first.getX()/bilder.first.width(), this->fensterhoehe/2-bildhoehe*marker.first.getY()/bilder.first.height()};
                    const pair<int, int> eckeUntenRechts{eckeObenLinks.first+bildbreite, eckeObenLinks.second+bildhoehe};
                    marker = {{(x-eckeObenLinks.first)/faktor.first, (y-eckeObenLinks.second)/faktor.first}, marker.second};
                    faktor = {faktor.first*this->faktorMultiplikator, faktor.second};
                    this->bildZeichnen(LINKES_BILD, bilder.first, renderer, marker.first, faktor.first);
                    SDL_UpdateWindowSurface(window);
                }
                else {
                    const double bildbreite{faktor.second*bilder.second.width()};
                    const double bildhoehe{faktor.second*bilder.second.height()};
                    const pair<int, int> eckeObenLinks{this->fensterhoehe/2-bildbreite*marker.second.getX()/bilder.second.width(), this->fensterhoehe/2-bildhoehe*marker.second.getY()/bilder.second.height()};
                    const pair<int, int> eckeUntenRechts{eckeObenLinks.first+bildbreite, eckeObenLinks.second+bildhoehe};
                    marker = {marker.first, {(x-this->fensterhoehe-eckeObenLinks.first)/faktor.second, (y-eckeObenLinks.second)/faktor.second}};
                    faktor = {faktor.first, faktor.second*this->faktorMultiplikator};
                    this->bildZeichnen(RECHTES_BILD, bilder.second, renderer, marker.second, faktor.second);
                    SDL_UpdateWindowSurface(window);
                }
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
    
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    for (int i = 0; i < this->fensterhoehe; ++i) {
        for (int j = 0; j < this->fensterhoehe; ++j) {
            SDL_RenderDrawPoint(renderer, i+seite*this->fensterhoehe, j);
        }
    }
    for (int i = max(eckeObenLinks.first, 0); i < min(this->fensterhoehe, eckeUntenRechts.first); ++i) {
        for (int j = max(eckeObenLinks.second, 0); j < min(this->fensterhoehe, eckeUntenRechts.second); ++j) {
            const int x{int((i-eckeObenLinks.first)/neuerFaktor)};
            const int y{int((j-eckeObenLinks.second)/neuerFaktor)};
            SDL_SetRenderDrawColor(renderer, (int)bild(x, y, 0, 0), (int)bild(x, y, 0, 1), (int)bild(x, y, 0, 2), 255);
            SDL_RenderDrawPoint(renderer, i+seite*this->fensterhoehe, j);
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = this->fensterhoehe/2-50; i < this->fensterhoehe/2+50+1; ++i) {
        SDL_RenderDrawPoint(renderer, i+seite*this->fensterhoehe, i);
        SDL_RenderDrawPoint(renderer, i+seite*this->fensterhoehe, this->fensterhoehe-i-1);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int j = 0; j < this->fensterhoehe; ++j) {
        SDL_RenderDrawPoint(renderer, this->fensterhoehe-1, j);
        SDL_RenderDrawPoint(renderer, this->fensterhoehe, j);
    }
    SDL_SetRenderDrawColor(renderer, 0, 120, 30, 255);
    for (int i = this->fensterhoehe-this->buttongroesse; i < this->fensterhoehe+this->buttongroesse+1; ++i) {
        for (int j = this->fensterhoehe-this->buttongroesse; j < this->fensterhoehe; ++j) {
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }
    SDL_RenderPresent(renderer);
}
