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
    const double hoeheMittelpunkt{this->suedpol/2};
    
    const double x{cos(input.getLon())*cos(input.getLat())*hoeheMittelpunkt};
    const double y{sin(input.getLon())*cos(input.getLat())*hoeheMittelpunkt};
    const double z{-hoeheMittelpunkt + sin(input.getLat())*hoeheMittelpunkt};
    
    const double faktor{this->suedpol/(z+this->suedpol)};
    return KartesischeKoordinaten(x*faktor, y*faktor);
}

const SphaerischeKoordinaten MapSphaerischToKartesisch::getUrbild(const KartesischeKoordinaten &input) const {
    const double lon{atan2(input.getY(), input.getX())};
    const double lat{M_PI/2-2*atan(sqrt(input.getX()*input.getX()+input.getY()*input.getY())/this->suedpol)};
    return {lon, lat};
}
