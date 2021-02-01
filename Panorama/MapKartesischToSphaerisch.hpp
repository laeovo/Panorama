#ifndef MapKartesischToSphaerisch_hpp
#define MapKartesischToSphaerisch_hpp

#include <stdio.h>

#include "KartesischeKoordinaten.hpp"
#include "SphaerischeKoordinaten.hpp"

class MapKartesischToSphaerisch {
public:
    const SphaerischeKoordinaten get(const KartesischeKoordinaten& input) const;
};

#endif /* MapKartesischToSphaerisch_hpp */
