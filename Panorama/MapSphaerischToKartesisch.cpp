#include <cmath>
#include <iostream>

#include "MapSphaerischToKartesisch.hpp"

using namespace std;

const KartesischeKoordinaten MapSphaerischToKartesisch::get(const SphaerischeKoordinaten& input) const {
    if (input.getLat() < -1.570796) {
        // Der Punkt liegt quasi auf dem Südpol
        cout << "MapSphaerischToKartesisch::get() :: Die Koordinaten liegen am Südpol und können nicht projiziert werden" << endl;
        assert(false);
        return {0, 0};
    }
    const double radius{(this->durchmesser)/2};
    assert(radius > 0);
    
    const double x{sin(input.getLon())*cos(input.getLat())*radius};
    const double y{-cos(input.getLon())*cos(input.getLat())*radius};
    const double z{-radius + sin(input.getLat())*radius};
    
    const double faktor{this->durchmesser/(z-(-this->durchmesser))};
    return KartesischeKoordinaten(x*faktor, y*faktor);
}

const SphaerischeKoordinaten MapSphaerischToKartesisch::getUrbild(const KartesischeKoordinaten &input) const {
    const double lon{atan2(input.getX(), -input.getY())};
    const double lat{M_PI/2-2*atan(sqrt(input.getX()*input.getX()+input.getY()*input.getY())/this->durchmesser)};
    return {lon, lat};
}
