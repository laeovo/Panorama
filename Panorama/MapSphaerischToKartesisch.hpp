#ifndef MapSphaerischToKartesisch_hpp
#define MapSphaerischToKartesisch_hpp

#include <stdio.h>

#include "KartesischeKoordinaten.hpp"
#include "SphaerischeKoordinaten.hpp"

class MapSphaerischToKartesisch {
public:
    MapSphaerischToKartesisch();
    MapSphaerischToKartesisch(const double startwinkel);
    const KartesischeKoordinaten get(const SphaerischeKoordinaten& input) const;
    const SphaerischeKoordinaten getUrbild(const KartesischeKoordinaten& input) const;
private:
    double durchmesser = 350;
    double startwinkel = 0; // Der Winkel, dessen Gerade nach unten zeigt (in -y Richtung)
};

#endif /* MapSphaerischToKartesisch_hpp */
