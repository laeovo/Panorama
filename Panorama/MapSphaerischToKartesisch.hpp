#ifndef MapSphaerischToKartesisch_hpp
#define MapSphaerischToKartesisch_hpp

#include <stdio.h>

#include "KartesischeKoordinaten.hpp"
#include "SphaerischeKoordinaten.hpp"

class MapSphaerischToKartesisch {
public:
    const KartesischeKoordinaten get(const SphaerischeKoordinaten& input) const;
    const SphaerischeKoordinaten getUrbild(const KartesischeKoordinaten& input) const;
private:
    double suedpol = 350;
};

#endif /* MapSphaerischToKartesisch_hpp */
