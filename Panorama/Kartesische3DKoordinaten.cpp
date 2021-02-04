#include <cmath>
#include <iostream>

#include "Kartesische3DKoordinaten.hpp"
#include "SphaerischeKoordinaten.hpp"

using namespace std;

Kartesische3DKoordinaten::Kartesische3DKoordinaten() : Kartesische3DKoordinaten(0, 0, 0) {}

Kartesische3DKoordinaten::Kartesische3DKoordinaten(const double x, const double y, const double z) : x(x), y(y), z(z) {}

Kartesische3DKoordinaten::Kartesische3DKoordinaten(const SphaerischeKoordinaten& spha) : x(cos(spha.getLon())*cos(spha.getLat())), y(sin(spha.getLon())*cos(spha.getLat())), z(sin(spha.getLat())) {}

Kartesische3DKoordinaten::Kartesische3DKoordinaten(const Kartesische3DKoordinaten& ort1, const Kartesische3DKoordinaten& ort2) : x(ort1.y*ort2.z-ort1.z*ort2.y), y(ort1.z*ort2.x-ort1.x*ort2.z), z(ort1.x*ort2.y-ort1.y*ort2.x) {}

const SphaerischeKoordinaten Kartesische3DKoordinaten::toSphaerisch() const {
    return {atan2(this->y, this->x), asin(this->z/this->getLaenge())};
}

const Kartesische3DKoordinaten Kartesische3DKoordinaten::operator-(const Kartesische3DKoordinaten& other) const {
    return {this->x-other.x, this->y-other.y, this->z-other.z};
}

const double Kartesische3DKoordinaten::getLaenge() const {
    return sqrt(this->x*this->x+this->y*this->y+this->z*this->z);
}

const Kartesische3DKoordinaten Kartesische3DKoordinaten::normiert() const {
    return {this->x/this->getLaenge(), this->y/this->getLaenge(), this->z/this->getLaenge()};
}

void Kartesische3DKoordinaten::print() const {
    cout << "3D: x = " << this->x << ", y = " << this->y << ", z = " << this->z << ", Länge: " << this->getLaenge() << endl;
}

const Kartesische3DKoordinaten rotationGegenUZS(const Kartesische3DKoordinaten& input, const Kartesische3DKoordinaten& rotationsvektor, const double winkel) {
    const Kartesische3DKoordinaten& n{rotationsvektor};
    const double laenge{n.getLaenge()};
    const double n_1{n.x/laenge};
    const double n_2{n.y/laenge};
    const double n_3{n.z/laenge};
    
    const double x{(n_1*n_1*(1-cos(winkel))+cos(winkel))*input.x + (n_1*n_2*(1-cos(winkel))-n_3*sin(winkel))*input.y + (n_1*n_3*(1-cos(winkel))+n_2*sin(winkel))*input.z};
    const double y{(n_2*n_1*(1-cos(winkel))+n_3*sin(winkel))*input.x + (n_2*n_2*(1-cos(winkel))+cos(winkel))*input.y + (n_2*n_3*(1-cos(winkel))-n_1*sin(winkel))*input.z};
    const double z{(n_3*n_1*(1-cos(winkel))-n_2*sin(winkel))*input.x + (n_3*n_2*(1-cos(winkel))+n_1*sin(winkel))*input.y + (n_3*n_3*(1-cos(winkel))+cos(winkel))*input.z};
    return {x, y, z};
}

const double skalarprodukt(const Kartesische3DKoordinaten& vektor1, const Kartesische3DKoordinaten& vektor2) {
    return vektor1.x*vektor2.x+vektor1.y*vektor2.y+vektor1.z*vektor2.z;
}

const double winkelZwischenVektoren(const Kartesische3DKoordinaten& vektor1, const Kartesische3DKoordinaten& vektor2) {
    return acos(skalarprodukt(vektor1, vektor2)/(vektor1.getLaenge()*vektor2.getLaenge()));
}
