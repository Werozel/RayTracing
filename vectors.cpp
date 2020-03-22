#include "vectors.h"
#include "objects.h"


float distance(const Point &p1, const Point &p2) {
    float x = p1.get_x() - p2.get_x();
    float y = p1.get_y() - p2.get_y();
    float z = p1.get_z() - p2.get_z();
    return x*x + y*y + z*z;
}