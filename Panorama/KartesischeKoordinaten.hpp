#ifndef KartesischeKoordinaten_hpp
#define KartesischeKoordinaten_hpp

#include <stdio.h>

class KartesischeKoordinaten {
public:
    KartesischeKoordinaten(const double x, const double y);
    const double getX() const;
    const double getY() const;
    void print() const;
private:
    double x;
    double y;
};

#endif /* KartesischeKoordinaten_hpp */
