#ifndef Marker_hpp
#define Marker_hpp

#include <stdio.h>
#include <string>

#include "KartesischeKoordinaten.hpp"
#include "SphaerischeKoordinaten.hpp"

class Marker {
public:
    Marker(const std::string& name);
    const std::string& getName() const;
private:
    const std::string name;
};


class MarkerKartesisch : public Marker {
public:
    MarkerKartesisch(const std::string& name, const KartesischeKoordinaten& koord);
    const KartesischeKoordinaten& getKoord() const;
    void setKoord(const KartesischeKoordinaten& neu);
private:
    KartesischeKoordinaten koord;
};


class MarkerSphaerisch : public Marker {
public:
    MarkerSphaerisch(const std::string& name, const SphaerischeKoordinaten& koord);
    const SphaerischeKoordinaten& getKoord() const;
    void setKoord(const SphaerischeKoordinaten& neu);
private:
    SphaerischeKoordinaten koord;
};

#endif /* Marker_hpp */
