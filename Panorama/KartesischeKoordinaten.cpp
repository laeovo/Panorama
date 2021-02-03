#include <iostream>

#include "KartesischeKoordinaten.hpp"

using namespace std;

KartesischeKoordinaten::KartesischeKoordinaten(const double x, const double y) : x(x), y(y) {}

const double KartesischeKoordinaten::getX() const {
    return this->x;
}

const double KartesischeKoordinaten::getY() const {
    return this->y;
}

void KartesischeKoordinaten::print() const {
    cout << "x = " << this->x << ", y = " << this->y << endl;
}
