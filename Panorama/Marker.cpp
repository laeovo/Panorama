#include "Marker.hpp"

using namespace std;

Marker::Marker(const string& name) : name(name) {}

const string& Marker::getName() const {
    return this->name;
}

MarkerKartesisch::MarkerKartesisch(const string& name, const KartesischeKoordinaten& koord) : Marker(name), koord(koord) {}

const KartesischeKoordinaten& MarkerKartesisch::getKoord() const {
    return this->koord;
}

void MarkerKartesisch::setKoord(const KartesischeKoordinaten& neu) {
    this->koord = neu;
}

MarkerSphaerisch::MarkerSphaerisch(const string& name, const SphaerischeKoordinaten& koord) : Marker(name), koord(koord) {}

const SphaerischeKoordinaten& MarkerSphaerisch::getKoord() const {
    return this->koord;
}

void MarkerSphaerisch::setKoord(const SphaerischeKoordinaten& neu) {
    this->koord = neu;
}
