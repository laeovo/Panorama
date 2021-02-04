#ifndef Kartesische3DKoordinaten_hpp
#define Kartesische3DKoordinaten_hpp

#include <stdio.h>

class SphaerischeKoordinaten;

struct Kartesische3DKoordinaten {
public:
    Kartesische3DKoordinaten();
    Kartesische3DKoordinaten(const double x, const double y, const double z);
    Kartesische3DKoordinaten(const SphaerischeKoordinaten& spha);
    Kartesische3DKoordinaten(const Kartesische3DKoordinaten& ort1, const Kartesische3DKoordinaten& ort2);
    const SphaerischeKoordinaten toSphaerisch() const;
    const Kartesische3DKoordinaten operator-(const Kartesische3DKoordinaten& other) const;
    
    const double getLaenge() const;
    const Kartesische3DKoordinaten normiert() const;
    void print() const;
    
    double x;
    double y;
    double z;
};

const Kartesische3DKoordinaten rotationGegenUZS(const Kartesische3DKoordinaten& input, const Kartesische3DKoordinaten& rotationsvektor, const double winkel);

const double skalarprodukt(const Kartesische3DKoordinaten& vektor1, const Kartesische3DKoordinaten& vektor2);

const double winkelZwischenVektoren(const Kartesische3DKoordinaten& vektor1, const Kartesische3DKoordinaten& vektor2);

#endif /* Kartesische3DKoordinaten_hpp */
