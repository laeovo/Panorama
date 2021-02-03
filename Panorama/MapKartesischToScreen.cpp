#include "MapKartesischToScreen.hpp"

using namespace std;

MapKartesischToScreen::MapKartesischToScreen() : MapKartesischToScreen(0, 0) {}

MapKartesischToScreen::MapKartesischToScreen(const int screenMitteX, const int screenMitteY) : screenMitteX(screenMitteX), screenMitteY(screenMitteY) {}

const ScreenKoordinaten MapKartesischToScreen::get(const KartesischeKoordinaten& input) const {
    return {int(input.getX())+this->screenMitteX, int(this->screenMitteY-input.getY())};
}

const KartesischeKoordinaten MapKartesischToScreen::getUrbild(const ScreenKoordinaten &input) const {
    return {(input.getX())+0.5-this->screenMitteX, this->screenMitteY-(input.getY()+0.5)};
}
