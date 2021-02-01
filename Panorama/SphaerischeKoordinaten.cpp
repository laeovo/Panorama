#include <iostream>
#include <cmath>

#include "SphaerischeKoordinaten.hpp"

using namespace std;

SphaerischeKoordinaten::SphaerischeKoordinaten(const double lon, const double lat) : lon(lon), lat(lat) {
    
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
    return acos(sin(other.getLat())*
                sin(this->getLat())+
                cos(other.getLat())*
                cos(this->getLat())*
                cos(other.getLon()-this->getLon()));
}
