#include <iostream>
#include <cmath>

#include "Kartesische3DKoordinaten.hpp"
#include "SphaerischeKoordinaten.hpp"

using namespace std;

SphaerischeKoordinaten::SphaerischeKoordinaten(const double lon, const double lat) : lon(lon), lat(lat) {
    while (this->lon < -M_PI) {
        this->lon += 2*M_PI;
    }
    while (this->lon > M_PI) {
        this->lon -= 2*M_PI;
    }
}

SphaerischeKoordinaten::SphaerischeKoordinaten(const Kartesische3DKoordinaten& koord3D) : lon(atan2(koord3D.y, koord3D.x)), lat(asin(koord3D.z/koord3D.getLaenge())) {
    
}

const double SphaerischeKoordinaten::getLon() const {
    return this->lon;
}

const double SphaerischeKoordinaten::getLat() const {
    return this->lat;
}

void SphaerischeKoordinaten::print() const {
    cout << "deg: (" << this->lon/M_PI*180 << ", " << this->lat/M_PI*180 << ")" << endl;
}

const double SphaerischeKoordinaten::angularDistance(const SphaerischeKoordinaten& other) const {
    return acos(sin(other.getLat()) * sin(this->getLat()) + cos(other.getLat()) * cos(this->getLat()) * cos(other.getLon()-this->getLon()));
}
