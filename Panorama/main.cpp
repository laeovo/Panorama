#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <vector>

#define cimg_display 0
#define cimg_use_jpeg

#include "CImg.h"
#include "SDL2/SDL.h"

#include "KartesischeKoordinaten.hpp"
#include "MapKartesischToScreen.hpp"
#include "MapSphaerischToKartesisch.hpp"
#include "Marker.hpp"
#include "MarkerTool.hpp"
#include "ScreenKoordinaten.hpp"
#include "SphaerischeKoordinaten.hpp"
#include "SphaerischesBild.hpp"
#include "Test.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    test1();
    return 0;
}
