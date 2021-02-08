#include "Marker.hpp"

using namespace std;

Marker::Marker(const size_t id) : id(id) {}

const size_t Marker::getId() const {
    return this->id;
}

MarkerKartesisch::MarkerKartesisch(const size_t id, const KartesischeKoordinaten& koord) : Marker(id), koord(koord) {}

const KartesischeKoordinaten& MarkerKartesisch::getKoord() const {
    return this->koord;
}

void MarkerKartesisch::setKoord(const KartesischeKoordinaten& neu) {
    this->koord = neu;
}

MarkerSphaerisch::MarkerSphaerisch(const size_t id, const SphaerischeKoordinaten& koord) : Marker(id), koord(koord) {}

const SphaerischeKoordinaten& MarkerSphaerisch::getKoord() const {
    return this->koord;
}

void MarkerSphaerisch::setKoord(const SphaerischeKoordinaten& neu) {
    this->koord = neu;
}
