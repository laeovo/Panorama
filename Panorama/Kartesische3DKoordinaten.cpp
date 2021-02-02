#include <cmath>
#include <iostream>

#include "Kartesische3DKoordinaten.hpp"

Kartesische3DKoordinaten::Kartesische3DKoordinaten() : Kartesische3DKoordinaten(0, 0, 0) {}

Kartesische3DKoordinaten::Kartesische3DKoordinaten(const double x, const double y, const double z) : x(x), y(y), z(z) {}

const Kartesische3DKoordinaten Kartesische3DKoordinaten::normiert() const {
    const double laenge{sqrt(pow(this->x, 2)+pow(this->y, 2)+pow(this->z, 2))};
    return {this->x/laenge, this->y/laenge, this->z/laenge};
}

const Kartesische3DKoordinaten rotationGegenUZS(const Kartesische3DKoordinaten& input, const Kartesische3DKoordinaten& rotationsvektor, const double winkel) {
    const Kartesische3DKoordinaten& n{rotationsvektor};
    const double n_1{n.x};
    const double n_2{n.y};
    const double n_3{n.z};
    
    const double x{(n_1*n_1*(1-cos(winkel))+cos(winkel))*input.x + (n_1*n_2*(1-cos(winkel))-n_3*sin(winkel))*input.y + (n_1*n_3*(1-cos(winkel))+n_2*sin(winkel))*input.z};
    const double y{(n_2*n_1*(1-cos(winkel))+n_3*sin(winkel))*input.x + (n_2*n_2*(1-cos(winkel))+cos(winkel))*input.y + (n_2*n_3*(1-cos(winkel))-n_1*sin(winkel))*input.z};
    const double z{(n_3*n_1*(1-cos(winkel))-n_2*sin(winkel))*input.x + (n_3*n_2*(1-cos(winkel))+n_1*sin(winkel))*input.y + (n_3*n_3*(1-cos(winkel))+cos(winkel))*input.z};
    return {x, y, z};
}
