#ifndef MapKartesischToScreen_hpp
#define MapKartesischToScreen_hpp

#include <stdio.h>

#include "KartesischeKoordinaten.hpp"
#include "ScreenKoordinaten.hpp"

class MapKartesischToScreen {
public:
    MapKartesischToScreen();
    MapKartesischToScreen(const int screenMitteX, const int screenMitteY);
    const ScreenKoordinaten get(const KartesischeKoordinaten& input) const;
    const KartesischeKoordinaten getUrbild(const ScreenKoordinaten& input) const;
private:
    const int screenMitteX;
    const int screenMitteY{};
};

#endif /* MapKartesischToScreen_hpp */
