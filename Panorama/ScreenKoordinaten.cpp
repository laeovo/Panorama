#include <iostream>

#include "ScreenKoordinaten.hpp"

using namespace std;

ScreenKoordinaten::ScreenKoordinaten(const int x, const int y) : x(x), y(y) {}

const int ScreenKoordinaten::getX() const {
    return this->x;
}

const int ScreenKoordinaten::getY() const {
    return this->y;
}

void ScreenKoordinaten::print() const {
    cout << "x = " << this->x << ", y = " << this->y << endl;
}
