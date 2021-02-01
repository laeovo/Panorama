#include "KartesischeKoordinaten.hpp"

KartesischeKoordinaten::KartesischeKoordinaten(const double x, const double y) : x(x), y(y) {}

const double KartesischeKoordinaten::getX() const {
    return this->x;
}

const double KartesischeKoordinaten::getY() const {
    return this->y;
}
