#ifndef ScreenKoordinaten_hpp
#define ScreenKoordinaten_hpp

#include <stdio.h>

class ScreenKoordinaten {
public:
    ScreenKoordinaten(const int x, const int y);
    const int getX() const;
    const int getY() const;
    void print() const;
private:
    int x;
    int y;
};

#endif /* ScreenKoordinaten_hpp */
