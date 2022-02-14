#ifndef VECTORS_H
#define VECTORS_H

#include <iostream>
#include <cmath>
#include "constants.h"


class Object;

class Sphere;

class Parallelepiped;

class Point;

class Vector;

class Triplet;

class RGB;

class Ray;


// Calculates distance between 2 points
float distance(const Point &p1, const Point &p2);

// Calculates a cos of an angle between 2 vectors
float get_angle(const Vector &v1, const Vector &v2);

// Calculates a sin of an angle between 2 vectors
float get_angle_sin(const Vector &v1, const Vector &v2);

float get_angle_sin(const float &cos);

// Calculates cross product of vectors
Vector cross_prod(const Vector &v1, const Vector &v2, bool invert_norm = false);


class Triplet {
public:
    float x;
    float y;
    float z;

    explicit Triplet(const float &n = 0.f) : x(n), y(n), z(n) {}

    Triplet(const float &tx, const float &ty, const float &tz) : x(tx), y(ty), z(tz) {}

    Triplet(const Triplet &t);

    [[nodiscard]] float get_x() const;

    [[nodiscard]] float get_y() const;

    [[nodiscard]] float get_z() const;

    Triplet& operator=(const Triplet &t);

    virtual bool operator==(const Triplet &v) const;

    Point operator+(const Triplet &v) const;

    void operator+=(const Triplet &t);

    Triplet operator-(const Triplet &v) const;

    Vector operator-() const;

    float operator*(const Triplet &v) const;

    Triplet operator*(const float &n) const;

    RGB operator/(const float &n) const;

    friend Triplet operator*(const float &n, const Triplet &t) {
        return Triplet{n * t.get_x(), n * t.get_y(), n * t.get_z()};
    }
};

class Vector : public Triplet {
public:
    explicit Vector(const float &n = 0.f) : Triplet(n) {}

    Vector(const float &tx, const float &ty, const float &tz) : Triplet(tx, ty, tz) {}

    explicit Vector(const Triplet &v);

    Vector(const Point &from, const Point &to);

    // Calculates a length of a current vector
    [[nodiscard]] float get_length() const;

    // Returns a normilized current vector
    [[nodiscard]] Vector normalize() const;

    friend Vector operator*(const float &n, const Vector &t) {
        return Vector{n * t.get_x(), n * t.get_y(), n * t.get_z()};
    }
};

class Point : public Triplet {
public:
    explicit Point(const float &n = 0.f) : Triplet(n) {}

    Point(const float &tx, const float &ty, const float &tz) : Triplet(tx, ty, tz) {}

    explicit Point(const Triplet &v) : Triplet(v) {}

    bool operator!=(const Point &p) const;

    bool operator==(const Point &p) const;
};

class RGB : public Triplet {
public:
    float r;
    float g;
    float b;

    explicit RGB(const float &n = 0.f) : Triplet(n), r(x), g(y), b(z) {}

    RGB(const float &tr, const float &tg, const float &tb) :
            Triplet(tr, tg, tb), r(x), g(y), b(z) {}

    explicit RGB(const Triplet &v) : Triplet(v), r(x), g(y), b(z) {}

    [[nodiscard]] float get_r() const;

    [[nodiscard]] float get_g() const;

    [[nodiscard]] float get_b() const;

    RGB& operator=(const RGB &v);

    RGB operator*(const float &n) const;

    RGB operator+(const RGB &rgb) const;

    bool operator==(const RGB &col) const;

    bool operator!=(const RGB &col) const;

    void normalize();

    friend std::ostream &operator<<(std::ostream &out, const RGB &v) {
        return out << (char) v.get_r() << (char) v.get_g() << (char) v.get_b();
    }
};


class Light {
public:
    Point position;
    float intensity;
    RGB color;

    explicit Light(const Point &pos, const float &intens = 1, const Colors &col = WHITE);

    [[nodiscard]] Point get_position() const;

    [[nodiscard]] float get_intensity() const;

    Light& operator=(const Light &light);
};


#endif