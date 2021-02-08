#ifndef MarkerTool_hpp
#define MarkerTool_hpp

#include <stdio.h>
#include <vector>

#include "KartesischeKoordinaten.hpp"

template<typename T>
class CImg;
class SDL_Renderer;

enum BildSeite {
    LINKES_BILD = 0,
    RECHTES_BILD = 1
};

class MarkerTool {
public:
    MarkerTool(const int fensterhoehe);
    const std::pair<KartesischeKoordinaten, KartesischeKoordinaten> markerBestimmen(const std::string& dateiname1, const std::string& dateiname2) const;
private:
    void bildZeichnen(const BildSeite seite, const cimg_library::CImg<unsigned char>& bild, SDL_Renderer* renderer, const KartesischeKoordinaten& neuerMarker, const double neuerFaktor) const;
    
    const int fensterhoehe;
};

#endif /* MarkerTool_hpp */
