#ifndef KugelTriangulation_hpp
#define KugelTriangulation_hpp

#include <stdio.h>
#include <vector>

#include "SphaerischeKoordinaten.hpp"

class KugelTriangulation {
public:
    KugelTriangulation(const double aufloesung);
    const unsigned int getRegion(const SphaerischeKoordinaten& koord) const;
    const unsigned int getAnzahlRegionen() const;
    void zeichnen() const;
private:
    const double aufloesung; // Laterale Auflösung (Höhe der Zellen)
    std::vector<unsigned int> zahlen; // Anzahl der Zellen pro Zeile
    std::vector<double> breiten; // Breiten der Zellen
};

#endif /* KugelTriangulation_hpp */
