#include <iostream>

#include "KugelTriangulation.hpp"

using namespace std;

KugelTriangulation::KugelTriangulation(const double aufloesung) : aufloesung(aufloesung) {
    for (double phi = M_PI/2-this->aufloesung; phi > -M_PI/2; phi -= this->aufloesung) {
        const double umfang{2*M_PI*cos(phi)};
        const int anzahl{int(umfang/aufloesung+1)};
        const double breite{2*M_PI/anzahl};
        this->zahlen.push_back(anzahl);
        this->breiten.push_back(breite);
    }
    cout << "Triangulation erstellt: " << this->zahlen.size() << " Zeilen mit " << this->getAnzahlRegionen() << " Regionen" << endl;
}

const unsigned int KugelTriangulation::getRegion(const SphaerischeKoordinaten &koord) const {
    unsigned int output{0};
    unsigned int counter{0};
    for (double lat = M_PI/2-this->aufloesung; lat > -M_PI/2; lat -= this->aufloesung) {
        if (lat > koord.getLat()) {
            output += zahlen[counter];
            ++counter;
        }
        else {
            return output + int((M_PI+koord.getLon())/this->breiten[counter]);
        }
    }
    return 0;
}

const unsigned int KugelTriangulation::getAnzahlRegionen() const {
    unsigned int output{0};
    for (const unsigned int anzahl : this->zahlen) {
        output += anzahl;
    }
    return output;
}