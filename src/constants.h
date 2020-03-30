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
    PURPLE
};

enum Materials {
    GLASS, 
    METAL, 
    PLASTIC
};

enum SurfaceType { OPAQUE, MIRROR, TRANSPARENT};
enum IntersectionType { ABSORPTION, REFLECTION, REFLECTION_AND_REFRACTION};

const int width = 1920;
const int height = 1080;

const float fov = 90;
const int recursion_gap = 5;

#endif