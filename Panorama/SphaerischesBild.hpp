#ifndef SphaerischesBild_hpp
#define SphaerischesBild_hpp

#include <stdio.h>
#include <string>
#include <vector>

#define cimg_display 0
#define cimg_use_jpeg

#include "CImg.h"

#include "Farbe.hpp"
#include "SphaerischeKoordinaten.hpp"

class SphaerischesBild {
public:
    SphaerischesBild(cimg_library::CImg<unsigned char> bild);
    const Farbe get(const SphaerischeKoordinaten& koordinaten) const;
private:
    std::vector<std::pair<SphaerischeKoordinaten, Farbe>> bild;
    const double schaerfe{0.01};
};

#endif /* SphaerischesBild_hpp */
