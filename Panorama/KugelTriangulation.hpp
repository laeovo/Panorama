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
private:
    const double aufloesung;
    std::vector<unsigned int> zahlen;
    std::vector<double> breiten;
};

#endif /* KugelTriangulation_hpp */
