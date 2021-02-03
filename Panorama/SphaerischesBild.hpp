#ifndef SphaerischesBild_hpp
#define SphaerischesBild_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include "Farbe.hpp"
#include "KugelTriangulation.hpp"
#include "MapKartesischToSphaerisch.hpp"
#include "SphaerischeKoordinaten.hpp"

class KartesischeKoordinaten;

class SphaerischesBild {
public:
    SphaerischesBild(const std::string& dateiname, const double brennweite, const double sensorbreite);
    const Farbe get(const SphaerischeKoordinaten& koordinaten) const;
    void print() const;
    void pan(const double winkel);
    void tilt(const double winkel);
    void drehenImUZS(const double winkel);
    void zentrumVerschieben(const SphaerischeKoordinaten& neuesZentrum);
    void markerHinzufuegen(const KartesischeKoordinaten& koord, const std::string& name);
    void markerHinzufuegen(const SphaerischeKoordinaten& koord, const std::string& name);
    const std::pair<SphaerischeKoordinaten, std::string>* const getMarker(const std::string& name) const;
private:
    std::vector<std::vector<std::pair<SphaerischeKoordinaten, Farbe>>> bild;
    std::vector<std::pair<SphaerischeKoordinaten, std::string>> marker;
    const double schaerfe{0.01}; // Einflussradius der Pixel.
    const KugelTriangulation triangulation{0.003}; // Unterteilung in Regionen. Der Parameter gibt die Regionengröße an. Vorzugsweise so, dass immer ca. 10 Pixel in einer Region sind.
    const MapKartesischToSphaerisch mapKartToSpha;
    double lon{0}; // Hiermit sind die Koordinaten des Bildreferenzpunkts gemeint. Initial in der Bildmitte.
    double lat{0}; // Hiermit sind die Koordinaten des Bildreferenzpunkts gemeint. Initial in der Bildmitte.
    bool wurdeTransformiert = false;
    
    void pixelAufRegionenAufteilen();
};

#endif /* SphaerischesBild_hpp */
