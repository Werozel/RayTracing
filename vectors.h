#ifndef __VECTORS_H__
#define __VECTORS_H__

#include <iostream>
#include <cmath>
#include "vectors.cpp"


float distance(const Point &p1, const Point &p2);


class Triplet {
    public:
        float x;
        float y;
        float z;

        Triplet(const float &n = 0.f): x(n), y(n), z(n) {}
        Triplet(const float &tx, const float &ty, const float &tz): x(tx), y(ty), z(tz) {}
        Triplet(const Triplet &t): x(t.x), y(t.y), z(t.z) {}

        virtual void operator= (const Triplet &v) { x = v.x; y = v.y; z = v.z;}
        inline bool operator== (const Triplet &v) const { return (x==v.x && y==v.y && z==v.z);}

        Triplet operator+ (const Triplet &v) const { return Triplet(x + v.x, y + v.y, z + v.z);}
        Triplet operator- (const Triplet &v) const { return Triplet(x - v.x, y - v.y, z - v.z);}
        Triplet operator-() const { return Triplet(-x, -y, -z);}
        float operator* (const Triplet &v) const { return x * v.x + y * v.y + z * v.z;}
        Triplet operator* (const float &n) const { return Triplet(x * n, y * n, z * n);}
        friend Triplet operator* (const float &n, const Triplet &v) { return Triplet(n * v.x, n * v.y, n * v.z);}
        friend std::ostream & operator<< (std::ostream &out, const Triplet &v) { return out << v.x << ", " << v.y << ", " << v.z;}
};

class Vector: public Triplet {
    public:
        float x;
        float y;
        float z;

        Vector(const float &n = 0.f): x(n), y(n), z(n) {}
        Vector(const float &tx, const float &ty, const float &tz): x(tx), y(ty), z(tz) {}
        Vector(const Vector &v): x(v.x), y(v.y), z(v.z) {}
        Vector(const Point &from, const Point &to): x(to.x - from.x), y(to.y - from.y), z(to.z - from.z) {}

        float get_length() {
            return x * x + y * y + z * z;
        }

        Vector normalize() {
            float l = get_length();
            return Vector(x / l, y / l, z / l);
        }
};

class Point: public Triplet {
    public:
        Point(const float &n = 0.f): Triplet(n) {}
        Point(const float &tx, const float &ty, const float &tz): Triplet(tx, ty, tz) {}
        Point(const Triplet &v): Triplet(v) {}

    static 
};

class RGB: public Triplet {
    public:
        float r;
        float g;
        float b;

        RGB(const float &n = 0.f): Triplet(n), r(x), g(y), b(z) {}
        RGB(const float &tr, const float &tg, const float &tb): Triplet(tr, tg, tb), r(x), g(y), b(z) {}
        RGB(const Triplet &v): Triplet(v), r(x), g(y), b(z) {}

        void operator= (const Triplet &v) { x = v.x; r = v.x; y = v.y; g = v.y; z = v.z; b = v.z;}

        friend std::ostream & operator<< (std::ostream &out, const RGB &v) { return out << (char)v.r << (char)v.g << (char)v.b;}
};


#endif