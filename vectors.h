#ifndef VECTORS_H
#define VECTORS_H

#include <iostream>
#include <cmath>


class Object;
class Sphere;
class Parallelepiped;
class Point;
class Vector;
class Triplet;
class RGB;
class Ray;


float distance(const Point &p1, const Point &p2);


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
        inline bool operator== (const Triplet &v) const;

        Triplet operator+ (const Triplet &v) const;
        Triplet operator- (const Triplet &v) const;
        Triplet operator-() const;
        float operator* (const Triplet &v) const;
        Triplet operator* (const float &n) const;
        friend Triplet operator* (const float &n, const Triplet &t) {return Triplet(n * t.get_x(), n * t.get_y(), n * t.get_z());}
};

class Vector: public Triplet {
    public:
        float x;
        float y;
        float z;

        Vector(const float &n = 0.f): x(n), y(n), z(n) {}
        Vector(const float &tx, const float &ty, const float &tz): x(tx), y(ty), z(tz) {}
        Vector(const Vector &v);
        Vector(const Point &from, const Point &to);

        float get_length() const;

        Vector normalize() const;
};

class Point: public Triplet {
    public:
        Point(const float &n = 0.f): Triplet(n) {}
        Point(const float &tx, const float &ty, const float &tz): Triplet(tx, ty, tz) {}
        Point(const Triplet &v): Triplet(v) {} 
};

class RGB: public Triplet {
    public:
        float r;
        float g;
        float b;

        RGB(const float &n = 0.f): Triplet(n), r(x), g(y), b(z) {}
        RGB(const float &tr, const float &tg, const float &tb): Triplet(tr, tg, tb), r(x), g(y), b(z) {}
        RGB(const Triplet &v): Triplet(v), r(x), g(y), b(z) {}

        float get_r() const;
        float get_g() const;
        float get_b() const;

        void operator= (const Triplet &v);

        friend std::ostream & operator<< (std::ostream &out, const RGB &v) { return out << (char)v.get_r() << (char)v.get_g() << (char)v.get_b();}
};


#endif