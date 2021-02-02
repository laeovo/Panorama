#ifndef SphaerischesBild_hpp
#define SphaerischesBild_hpp

#include <stdio.h>
#include <string>
#include <vector>

#define cimg_display 0
#define cimg_use_jpeg

#include "CImg.h"

#include "Farbe.hpp"
#include "KugelTriangulation.hpp"
#include "SphaerischeKoordinaten.hpp"

class SphaerischesBild {
public:
    SphaerischesBild(cimg_library::CImg<unsigned char> bild);
    SphaerischesBild(const std::string& dateiname, const double brennweite, const double sensorbreite);
    const Farbe get(const SphaerischeKoordinaten& koordinaten) const;
    void print() const;
    void pan(const double winkel);
    void tilt(const double winkel);
    void zentrumVerschieben(const SphaerischeKoordinaten& neuesZentrum);
    void drehenImUZS(const double winkel);
private:
    std::vector<std::vector<std::pair<SphaerischeKoordinaten, Farbe>>> bild;
    const double schaerfe{0.01}; // Einflussradius der Pixel.
    const KugelTriangulation triangulation{0.003}; // Unterteilung in Regionen. Der Parameter gibt die Regionengröße an. Vorzugsweise so, dass immer ca. 10 Pixel in einer Region sind.
    double lon{0}; // Hiermit sind die Koordinaten des Bildmittelpunktes gemeint
    double lat{0}; // Hiermit sind die Koordinaten des Bildmittelpunktes gemeint
    
    void pixelAufRegionenAufteilen();
};

#endif /* SphaerischesBild_hpp */
