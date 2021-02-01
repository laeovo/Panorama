#ifndef Farbe_hpp
#define Farbe_hpp

#include <stdio.h>

struct Farbe {
public:
    Farbe(const unsigned char r, const unsigned char g, const unsigned char b);
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

#endif /* Farbe_hpp */
