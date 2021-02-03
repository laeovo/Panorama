#ifndef MapKartesischToSphaerisch_hpp
#define MapKartesischToSphaerisch_hpp

#include <stdio.h>

#include "KartesischeKoordinaten.hpp"
#include "SphaerischeKoordinaten.hpp"

class MapKartesischToSphaerisch {
public:
    MapKartesischToSphaerisch(const int aufloesungX, const int aufloesungY, const double brennweite, const double sensorbreite);
    const SphaerischeKoordinaten get(const KartesischeKoordinaten& input) const;
private:
    // Parameter
    const int aufloesungX;
    const int aufloesungY;
    const double brennweite;
    const double sensorbreite;
    
    // berechnet
    const double projektionsebene;
    const double eckeObenLinksY;
    const double eckeObenLinksZ;
    const double eckeUntenRechtsY;
    const double eckeUntenRechtsZ;
};

#endif /* MapKartesischToSphaerisch_hpp */
