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


class Triplet {
    public:
        float x;
        float y;
        float z;

        Triplet(const float &n = 0.f): x(n), y(n), z(n) {}
        Triplet(const float &tx, const float &ty, const float &tz): x(tx), y(ty), z(tz) {}
        Triplet(const Triplet &t);

        float get_x() const;
        float get_y() const;
        float get_z() const;

        virtual void operator= (const Triplet &t);
        virtual bool operator== (const Triplet &v) const;

        Triplet operator+ (const Triplet &v) const;
        Triplet operator- (const Triplet &v) const;
        Triplet operator-() const;
        float operator* (const Triplet &v) const;
        Triplet operator* (const float &n) const;
        friend Triplet operator* (const float &n, const Triplet &t) 
            {return Triplet(n * t.get_x(), n * t.get_y(), n * t.get_z());}
};

class Vector: public Triplet {
    public:
        Vector(const float &n = 0.f): Triplet(n) {}
        Vector(const float &tx, const float &ty, const float &tz): Triplet(tx, ty, tz) {}
        Vector(const Triplet &v);
        Vector(const Point &from, const Point &to);

        // Calculates a length of a current vector
        float get_length() const;

        // Returns a normilized current vector
        Vector normalize() const;
};

class Point: public Triplet {
    public:
        Point(const float &n = 0.f): Triplet(n) {}
        Point(const float &tx, const float &ty, const float &tz): Triplet(tx, ty, tz) {}
        Point(const Triplet &v): Triplet(v) {} 

        bool operator!= (const Point &p) const;
        bool operator== (const Point &p) const;
};

class RGB: public Triplet {
    public:
        float r;
        float g;
        float b;

        RGB(const float &n = 0.f): Triplet(n), r(x), g(y), b(z) {}
        RGB(const float &tr, const float &tg, const float &tb): 
                                    Triplet(tr, tg, tb), r(x), g(y), b(z) {}
        RGB(const Triplet &v): Triplet(v), r(x), g(y), b(z) {}

        float get_r() const;
        float get_g() const;
        float get_b() const;

        void operator= (const Triplet &v);
        RGB operator* (const float &n) const;
        RGB operator+ (const RGB &rgb) const;

        bool operator== (const RGB &col) const;
        bool operator!= (const RGB &col) const;

        friend std::ostream & operator<< (std::ostream &out, const RGB &v) 
            { return out << (char)v.get_r() << (char)v.get_g() << (char)v.get_b();}
};


class Light {
    public:
        Point position;
        float intensity;
        RGB color;

        Light(const Point &pos, const float &intens = 1, const Colors &col = WHITE);  

        Point get_position() const;
        float get_intensity() const;
        RGB get_light_color() const;

        void operator= (const Light &light);
};


#endif