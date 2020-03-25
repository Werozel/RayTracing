#include "vectors.h"
#include "objects.h"
#include <cmath>


float distance(const Point &p1, const Point &p2) {
    float x = p1.get_x() - p2.get_x();
    float y = p1.get_y() - p2.get_y();
    float z = p1.get_z() - p2.get_z();
    return sqrt(x*x + y*y + z*z);
}

float get_angle(const Vector &v1, const Vector &v2) {
    return v1 * v2 / v1.get_length() / v2.get_length();
}


// -------------------- Triplet ----------------------------
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


// ------------------------ Point --------------------------
bool Point::operator!= (const Point &p) const { return (x != p.get_x() || y != p.get_y() || z != p.get_z());}


// --------------------- Vector ----------------------------
Vector::Vector(const Point &from, const Point &to): Triplet(to.get_x() - from.get_x(), to.get_y() - from.get_y(),to.get_z() - from.get_z()) {}
Vector::Vector(const Triplet &v): Triplet(v) {}


float Vector::get_length() const {
    return sqrt(x * x + y * y + z * z);
}

Vector Vector::normalize() const {
    float l = get_length();
    return Vector(x / l, y / l, z / l);
}


// ------------------- RGB ----------------------------
float RGB::get_r() const {return r;}
float RGB::get_g() const {return g;}
float RGB::get_b() const {return b;}

void RGB::operator= (const Triplet &v) { x = r = v.get_x(); y = g = v.get_y(); z = b = v.get_z();}

bool RGB::operator== (const RGB &col) const {
    return (r == col.get_r() && g == col.get_g() && b == col.get_b());
}

bool RGB::operator!= (const RGB &col) const {
    return (r != col.get_r() || g != col.get_g() || b != col.get_b());
}

RGB RGB::operator* (const float &n) const {
    float new_r = std::min<float>(255, n * r);
    float new_g = std::min<float>(255, n * g);
    float new_b = std::min<float>(255, n * b);
    return RGB(new_r, new_g, new_b);
}


// ------------------ Light -------------------------
Light::Light (const Point &pos, const float &intens = 1): position(pos), intensity(intens) {}

Point Light::get_position() const { return position;} 
float Light::get_intensity() const { return intensity;}

void Light::operator= (const Light &light) { position = light.get_position(); intensity = get_intensity();}


