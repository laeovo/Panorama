#ifndef Kartesische3DKoordinaten_hpp
#define Kartesische3DKoordinaten_hpp

#include <stdio.h>

struct Kartesische3DKoordinaten {
public:
    Kartesische3DKoordinaten();
    Kartesische3DKoordinaten(const double x, const double y, const double z);
    
    const Kartesische3DKoordinaten normiert() const;
    
    double x;
    double y;
    double z;
};

const Kartesische3DKoordinaten rotationGegenUZS(const Kartesische3DKoordinaten& input, const Kartesische3DKoordinaten& rotationsvektor, const double winkel);

#endif /* Kartesische3DKoordinaten_hpp */
