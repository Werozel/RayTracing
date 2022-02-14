#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

enum Colors {
    BG_COLOR,
    WHITE,
    BLACK,
    GRAY,
    RED,
    GREEN,
    BLUE,
    LIGHT_BLUE,
    PURPLE,
    DARK_PURPLE,
    YELLOW,
    ORANGE,
    BROWN,
    PINK,
    DARK_PINK
};

enum Materials {
    GLASS,
    METAL,
    PLASTIC,
    COLORED_METAL,
    MARBLE
};

enum SurfaceType {
    OPAQUE,
    MIRROR,
    TRANSPARENT,
    COLORED_MIRROR
};
enum IntersectionType {
    ABSORPTION,
    REFLECTION,
    REFLECTION_AND_REFRACTION
};


const int width = 1920;
const int height = 1080;

const int recursion_gap = 4;

#endif