#ifndef Marker_hpp
#define Marker_hpp

#include <stdio.h>
#include <string>

#include "KartesischeKoordinaten.hpp"
#include "SphaerischeKoordinaten.hpp"

class Marker {
public:
    Marker(const size_t id);
    const size_t getId() const;
private:
    const size_t id;
};


class MarkerKartesisch : public Marker {
public:
    MarkerKartesisch(const size_t id, const KartesischeKoordinaten& koord);
    const KartesischeKoordinaten& getKoord() const;
    void setKoord(const KartesischeKoordinaten& neu);
private:
    KartesischeKoordinaten koord;
};


class MarkerSphaerisch : public Marker {
public:
    MarkerSphaerisch(const size_t id, const SphaerischeKoordinaten& koord);
    const SphaerischeKoordinaten& getKoord() const;
    void setKoord(const SphaerischeKoordinaten& neu);
private:
    SphaerischeKoordinaten koord;
};

#endif /* Marker_hpp */
