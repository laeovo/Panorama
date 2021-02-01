#include "SphaerischesBild.hpp"

using namespace std;

SphaerischesBild::SphaerischesBild(cimg_library::CImg<unsigned char> bild) {
    const double aufloesungLon{2*M_PI / bild.width()};
    const double aufloesungLat{M_PI / bild.height()};
    for (size_t i = 0; i < bild.width(); ++i) {
        for (size_t j = 0; j < bild.height(); ++j) {
            const SphaerischeKoordinaten koord{-M_PI + i*aufloesungLon, -M_PI/2 + (bild.height()-j-1)*aufloesungLat};
            const Farbe farbe{bild(i, j, 0, 0), bild(i, j, 0, 1), bild(i, j, 0, 2)};
            this->bild.push_back({koord, farbe});
        }
    }
}

const Farbe SphaerischesBild::get(const SphaerischeKoordinaten& koordinaten) const {
    double gesamtgewicht{0};
    Farbe output{0, 0, 0};
    for (const pair<SphaerischeKoordinaten, Farbe>& datenpaar : this->bild) {
        const SphaerischeKoordinaten& koord{datenpaar.first};
        const Farbe& farbe{datenpaar.second};
        const double entfernung{koordinaten.angularDistance(koord)};
        if (entfernung < this->schaerfe) {
            const double gewicht{this->schaerfe - entfernung};
            output.r += gewicht * farbe.r;
            output.g += gewicht * farbe.g;
            output.b += gewicht * farbe.b;
            gesamtgewicht += this->schaerfe-entfernung;
        }
    }
    output.r /= gesamtgewicht;
    output.g /= gesamtgewicht;
    output.b /= gesamtgewicht;
    return output;
}
