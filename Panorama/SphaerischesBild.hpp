#ifndef SphaerischesBild_hpp
#define SphaerischesBild_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include "Farbe.hpp"
#include "KugelTriangulation.hpp"
#include "MapKartesischToSphaerisch.hpp"
#include "Marker.hpp"
#include "SphaerischeKoordinaten.hpp"

class Kartesische3DKoordinaten;
class KartesischeKoordinaten;
class MarkerKartesisch;

class SphaerischesBild {
public:
    SphaerischesBild(const std::string& dateiname, const double brennweite, const double sensorbreite);
    const Farbe get(const SphaerischeKoordinaten& koordinaten) const;
    void print() const;
    void allesUmAchseDrehen(const Kartesische3DKoordinaten& drehachse, const double winkel);
    void pan(const double winkel);
    void tilt(const double winkel);
    void drehenImUZS(const double winkel);
    void ausrichten(const SphaerischeKoordinaten& marker1, const SphaerischeKoordinaten& marker2, const SphaerischeKoordinaten& referenz1, const SphaerischeKoordinaten& referenz2);
    void zentrumVerschieben(const SphaerischeKoordinaten& neuesZentrum);
    void markerHinzufuegen(const MarkerKartesisch& neuerMarker);
    void markerHinzufuegen(const MarkerSphaerisch& neuerMarker);
    const MarkerSphaerisch* const getMarker(const std::string& name) const;
    const std::vector<MarkerSphaerisch>& getAlleMarker() const;
private:
    std::vector<std::vector<std::pair<SphaerischeKoordinaten, Farbe>>> bild;
    std::vector<MarkerSphaerisch> marker;
    const double schaerfe{0.01}; // Einflussradius der Pixel.
    const KugelTriangulation triangulation{0.003}; // Unterteilung in Regionen. Der Parameter gibt die Regionengröße an. Vorzugsweise so, dass immer ca. 10 Pixel in einer Region sind.
    const MapKartesischToSphaerisch mapKartToSpha;
    SphaerischeKoordinaten mitte{0, 0}; // Hiermit sind die Koordinaten des Bildreferenzpunkts gemeint. Initial in der Bildmitte.
    bool wurdeTransformiert = false;
    
    void pixelAufRegionenAufteilen();
};

#endif /* SphaerischesBild_hpp */
