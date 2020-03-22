#include "vectors.h"
#include "objects.h"


float distance(const Point &p1, const Point &p2) {
    float x = p1.x - p2.x;
    float y = p1.y - p2.y;
    float z = p1.z - p2.z;
    return x*x + y*y + z*z;
}