#include <cmath>
#include <iostream>

#include "MapKartesischToSphaerisch.hpp"

using namespace std;

MapKartesischToSphaerisch::MapKartesischToSphaerisch(const int aufloesungX, const int aufloesungY, const double brennweite, const double sensorbreite) : aufloesungX(aufloesungX), aufloesungY(aufloesungY), brennweite(brennweite), sensorbreite(sensorbreite), projektionsebene(sqrt(1-pow(2*sin(0.5*(2*atan(0.5*(this->sensorbreite*sqrt(1+pow((0.+this->aufloesungY)/this->aufloesungX,2)))/this->brennweite)))/2,2))), eckeObenLinksY(sqrt((1-pow(sqrt(1-pow(2*sin(0.5*(2*atan(0.5*(this->sensorbreite*sqrt(1+pow((0.+this->aufloesungY)/this->aufloesungX,2)))/this->brennweite)))/2,2)),2))/(pow((0.+this->aufloesungY)/this->aufloesungX,2)+1))), eckeObenLinksZ(this->eckeObenLinksY*((0.+this->aufloesungY)/this->aufloesungX)), eckeUntenRechtsY(-this->eckeObenLinksY), eckeUntenRechtsZ(-this->eckeObenLinksZ) {}

const SphaerischeKoordinaten MapKartesischToSphaerisch::get(const KartesischeKoordinaten& input) const {
    // Kartesische Koordinaten berechnen
    const double x{this->projektionsebene};
    const double y{this->eckeObenLinksY+((0.+input.getX())/this->aufloesungX)*(this->eckeUntenRechtsY-this->eckeObenLinksY)};
    const double z{this->eckeObenLinksZ+((0.+input.getY())/this->aufloesungY)*(this->eckeUntenRechtsZ-this->eckeObenLinksZ)};
    
    // TODO: Verzeichnung korrigieren?
    
    // projizierte Koordinaten berechnen
    const double faktor{1/sqrt(pow(x,2)+pow(y,2)+pow(z,2))};
    const double xProjiziert{x*faktor};
    const double yProjiziert{y*faktor};
    const double zProjiziert{z*faktor};
    
    // spährische Koordinaten berechnen
    const double lambda{atan2(yProjiziert, xProjiziert)};
    const double phi{asin(zProjiziert)};
    
    return{lambda,phi};
}
