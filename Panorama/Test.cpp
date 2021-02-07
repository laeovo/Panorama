#include <cmath>
#include <iostream>

#include "SDL2/SDL.h"

#include "Kartesische3DKoordinaten.hpp"
#include "SphaerischeKoordinaten.hpp"
#include "Test.hpp"

using namespace std;

Test::Test() {
//    SphaerischeKoordinaten marker1{-0.30516082, 0.3727735};
//    SphaerischeKoordinaten marker2{-0.33890025, -0.18870953};
//    SphaerischeKoordinaten referenz1{-0.58075635, 0.36714478};
//    SphaerischeKoordinaten referenz2{-0.61490829, -0.1935833};
    SphaerischeKoordinaten marker1{-0.3486, 1.3};
    SphaerischeKoordinaten marker2{1.56, -0.23};
    SphaerischeKoordinaten referenz1{rotationGegenUZS(marker1, {1.5, -3.4, 1.89}, 1.4)};
    SphaerischeKoordinaten referenz2{rotationGegenUZS(marker2, {1.5, -3.4, 1.89}, 1.4)};
    
//    referenz2 = rotationGegenUZS(referenz2, referenz1, -0.5);
    
    const Kartesische3DKoordinaten mittelebene1{Kartesische3DKoordinaten(referenz1) - Kartesische3DKoordinaten(marker1)};
    const Kartesische3DKoordinaten mittelebene2{Kartesische3DKoordinaten(referenz2) - Kartesische3DKoordinaten(marker2)};
    const Kartesische3DKoordinaten drehachse{Kartesische3DKoordinaten(mittelebene1, mittelebene2).normiert()};
    
    const double alpha1{winkelZwischenVektoren(drehachse, marker1)};
    const double abschnittAufDrehachse1{cos(alpha1)};
    const Kartesische3DKoordinaten punktAufDrehachse1{drehachse.x * abschnittAufDrehachse1, drehachse.y * abschnittAufDrehachse1, drehachse.z * abschnittAufDrehachse1};
    const Kartesische3DKoordinaten richtungsvektorMarker1{Kartesische3DKoordinaten(marker1) - punktAufDrehachse1};
    const Kartesische3DKoordinaten richtungsvektorReferenz1{Kartesische3DKoordinaten(referenz1) - punktAufDrehachse1};
    double winkel1{winkelZwischenVektoren(richtungsvektorMarker1, richtungsvektorReferenz1)};
    const Kartesische3DKoordinaten drehachseAusMarkern1{richtungsvektorMarker1, richtungsvektorReferenz1};
    
    const double alpha2{winkelZwischenVektoren(drehachse, marker2)};
    const double abschnittAufDrehachse2{cos(alpha2)};
    const Kartesische3DKoordinaten punktAufDrehachse2{drehachse.x * abschnittAufDrehachse2, drehachse.y * abschnittAufDrehachse2, drehachse.z * abschnittAufDrehachse2};
    const Kartesische3DKoordinaten richtungsvektorMarker2{Kartesische3DKoordinaten(marker2) - punktAufDrehachse2};
    const Kartesische3DKoordinaten richtungsvektorReferenz2{Kartesische3DKoordinaten(referenz2) - punktAufDrehachse2};
    double winkel2{winkelZwischenVektoren(richtungsvektorMarker2, richtungsvektorReferenz2)};
    const Kartesische3DKoordinaten drehachseAusMarkern2{richtungsvektorMarker2, richtungsvektorReferenz2};
    
    cout << "Abschnitt 1: " << abschnittAufDrehachse1 << endl;
    cout << "Abschnitt 2: " << abschnittAufDrehachse2 << endl;
    
    
    cout << "Winkel 1 = " << winkel1 << endl;
    cout << "Winkel 2 = " << winkel2 << endl;
    cout << "Durchschnitt = " << ((winkel1+winkel2)/2) << endl;
    
    if (skalarprodukt(drehachse, drehachseAusMarkern1) > 0) {
        // gegen den UZS drehen
    }
    else {
        // im UZS drehen
        winkel1 *= -1;
    }
    cout << "Abstand Marker vs. Referenz: " << marker1.angularDistance(marker2) << " vs. " << referenz1.angularDistance(referenz2) << ", Differenz = " << referenz1.angularDistance(referenz2) - marker1.angularDistance(marker2) << endl;
    
    cout << "Abstand Marker1 - Referenz1: " << marker1.angularDistance(referenz1) << " --> " << SphaerischeKoordinaten(rotationGegenUZS(marker1, drehachse, (winkel1+winkel2)/2)).angularDistance(referenz1) << endl;
    cout << "Abstand Marker2 - Referenz2: " << marker2.angularDistance(referenz2) << " --> " << SphaerischeKoordinaten(rotationGegenUZS(marker2, drehachse, (winkel1+winkel2)/2)).angularDistance(referenz2) << endl;
    
    // Hintergrund
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Panorama", 0, 0, 1000, 500, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    for (int i = -2; i <= 2; ++i) {
        for (int j = -2; j <= 2; ++j) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawPoint(renderer, marker1.getLon()*1000/2/M_PI+500+i, -marker1.getLat()*500/M_PI+250+j);
            SDL_SetRenderDrawColor(renderer, 153, 0, 0, 255);
            SDL_RenderDrawPoint(renderer, referenz1.getLon()*1000/2/M_PI+500+i, -referenz1.getLat()*500/M_PI+250+j);
        }
    }
    
    for (int i = -2; i <= 2; ++i) {
        for (int j = -2; j <= 2; ++j) {
            SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
            SDL_RenderDrawPoint(renderer, marker2.getLon()*1000/2/M_PI+500+i, -marker2.getLat()*500/M_PI+250+j);
            SDL_SetRenderDrawColor(renderer, 153, 120, 0, 255);
            SDL_RenderDrawPoint(renderer, referenz2.getLon()*1000/2/M_PI+500+i, -referenz2.getLat()*500/M_PI+250+j);
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = -2; i <= 2; ++i) {
        for (int j = -2; j <= 2; ++j) {
            SDL_RenderDrawPoint(renderer, SphaerischeKoordinaten(drehachse).getLon()*1000/2/M_PI+500+i, -SphaerischeKoordinaten(drehachse).getLat()*500/M_PI+250+j);
            SDL_RenderDrawPoint(renderer, SphaerischeKoordinaten(drehachse-drehachse-drehachse).getLon()*1000/2/M_PI+500+i, -SphaerischeKoordinaten(drehachse-drehachse-drehachse).getLat()*500/M_PI+250+j);
        }
    }
    
    for (size_t i = 0; i < 1000; ++i) {
        SDL_RenderDrawPoint(renderer, i, 250);
    }
    for (size_t j = 0; j < 500; ++j) {
        SDL_RenderDrawPoint(renderer, 500, j);
    }
    
    for (double i = 0; i < 2*M_PI; i += 0.05) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        const Kartesische3DKoordinaten ebenenpunkt1{mittelebene1.y, -mittelebene1.x, 0};
        const SphaerischeKoordinaten spurpunkt1{rotationGegenUZS(ebenenpunkt1, mittelebene1, i)};
        SDL_RenderDrawPoint(renderer, spurpunkt1.getLon()*1000/2/M_PI+500, -spurpunkt1.getLat()*500/M_PI+250);
        
        SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
        const Kartesische3DKoordinaten ebenenpunkt2{mittelebene2.y, -mittelebene2.x, 0};
        const SphaerischeKoordinaten spurpunkt2{rotationGegenUZS(ebenenpunkt2, mittelebene2, i)};
        SDL_RenderDrawPoint(renderer, spurpunkt2.getLon()*1000/2/M_PI+500, -spurpunkt2.getLat()*500/M_PI+250);
    }
    
    for (double i = 0; i < 2*M_PI; i += 0.005) {
        const Kartesische3DKoordinaten marker3{drehachse.y, -drehachse.x, 0};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        const SphaerischeKoordinaten spurpunkt3{rotationGegenUZS(marker3, drehachse, i)};
        SDL_RenderDrawPoint(renderer, spurpunkt3.getLon()*1000/2/M_PI+500, -spurpunkt3.getLat()*500/M_PI+250);
    }
    
    for (double i = 0; i < max(winkel1, winkel2) && i > -max(winkel1, winkel2); i += 0.005*winkel1/abs(winkel1)) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        const SphaerischeKoordinaten spurpunkt1{rotationGegenUZS(marker1, drehachse, i)};
        SDL_RenderDrawPoint(renderer, spurpunkt1.getLon()*1000/2/M_PI+500, -spurpunkt1.getLat()*500/M_PI+250);
        
        SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
        const SphaerischeKoordinaten spurpunkt2{rotationGegenUZS(marker2, drehachse, i)};
        SDL_RenderDrawPoint(renderer, spurpunkt2.getLon()*1000/2/M_PI+500, -spurpunkt2.getLat()*500/M_PI+250);
    }



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
    cout << "---------------------------------------------------------------" << endl;
    assert(false);
}
