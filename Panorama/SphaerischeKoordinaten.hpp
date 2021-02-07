#ifndef SphaerischeKoordinaten_hpp
#define SphaerischeKoordinaten_hpp

#include <stdio.h>

class Kartesische3DKoordinaten;

class SphaerischeKoordinaten {
public:
    SphaerischeKoordinaten(const double lon, const double lat);
    SphaerischeKoordinaten(const Kartesische3DKoordinaten& koord3D);
    const double getLon() const;
    const double getLat() const;
    void print() const;
    const double angularDistance(const SphaerischeKoordinaten& other) const;
private:
    double lon; // radians
    double lat; // radians
};


#endif /* SphaerischeKoordinaten_hpp */
