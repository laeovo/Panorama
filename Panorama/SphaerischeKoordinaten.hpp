#ifndef SphaerischeKoordinaten_hpp
#define SphaerischeKoordinaten_hpp

#include <stdio.h>

class SphaerischeKoordinaten {
public:
    SphaerischeKoordinaten(const double lon, const double lat);
    const double getLon() const;
    const double getLat() const;
    void print() const;
    const double angularDistance(const SphaerischeKoordinaten& other) const;
private:
    double lon; // radians
    double lat; // radians
};


#endif /* SphaerischeKoordinaten_hpp */
