#include "vectors.h"
#include "objects.h"
#include <cmath>


float distance(const Point &p1, const Point &p2) {
    float x = p1.get_x() - p2.get_x();
    float y = p1.get_y() - p2.get_y();
    float z = p1.get_z() - p2.get_z();
    return x*x + y*y + z*z;
}

Triplet::Triplet(const Triplet &t): x(t.get_x()), y(t.get_y()), z(t.get_z()) {}


float Triplet::get_x() const {return x;}
float Triplet::get_y() const {return y;}
float Triplet::get_z() const {return z;}

void Triplet::operator= (const Triplet &t) { x = t.get_x(); y = t.get_y(); z = t.get_z();}
inline bool Triplet::operator== (const Triplet &v) const { return (x==v.get_x() && y==v.get_y() && z==v.get_z());}

Triplet Triplet::operator+ (const Triplet &v) const { return Triplet(x + v.get_x(), y + v.get_y(), z + v.get_z());}
Triplet Triplet::operator- (const Triplet &v) const { return Triplet(x - v.get_x(), y - v.get_y(), z - v.get_z());}
Triplet Triplet::operator-() const { return Triplet(-x, -y, -z);}
float Triplet::operator* (const Triplet &v) const { return x * v.get_x() + y * v.get_y() + z * v.get_z();}
Triplet Triplet::operator* (const float &n) const { return Triplet(x * n, y * n, z * n);}


Vector::Vector(const Point &from, const Point &to): Triplet(to.get_x() - from.get_x(), to.get_y() - from.get_y(),to.get_z() - from.get_z()) {}
Vector::Vector(const Triplet &v): Triplet(v) {}


float Vector::get_length() const {
    return sqrt(x * x + y * y + z * z);
}

Vector Vector::normalize() const {
    float l = get_length();
    return Vector(x / l, y / l, z / l);
}


float RGB::get_r() const {return r;}
float RGB::get_g() const {return g;}
float RGB::get_b() const {return b;}

void RGB::operator= (const Triplet &v) { x = r = v.get_x(); y = g = v.get_y(); z = b = v.get_z();}