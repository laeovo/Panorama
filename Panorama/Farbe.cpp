#include <iostream>

#include "Farbe.hpp"

using namespace std;

Farbe::Farbe(const unsigned char r, const unsigned char g, const unsigned char b) : r(r), g(g), b(b) {
    
}

void Farbe::print() const {
    cout << "Farbe: r=" << (int)this->r << ", g=" << (int)this->g << ", b=" << (int)this->b << endl;
}
