#ifndef __VECTORS_H__
#define __VECTORS_H__

#include <cmath>
#include <vector>
#include <iostream>

class Vector {
    public:
        float x;
        float y;
        float z;

        Vector(): x(0.f), y(0.f), z(0.f) {}
        Vector(const float &n): x(n), y(n), z(n) {}
        Vector(const float &tx, const float &ty, const float &tz): x(tx), y(ty), z(tz) {}
        Vector(const Vector &v): x{v.x}, y{v.y}, z{v.z} {}

        virtual void operator= (const Vector &v) { x = v.x; y = v.y; z = v.z;}
        inline bool operator== (const Vector &v) const { return (x==v.x && y==v.y && z==v.z);}

        Vector operator+ (const Vector &v) const { return Vector(x + v.x, y + v.y, z + v.z);}
        Vector operator- (const Vector &v) const { return Vector(x - v.x, y - v.y, z - v.z);}
        Vector operator-() const { return Vector(-x, -y, -z);}
        Vector operator* (const Vector &v) const { return Vector(x * v.x, y * v.y, z * v.z);}
        Vector operator* (const float &n) const { return Vector(x * n, y * n, z * n);}
        friend Vector operator* (const float &n, const Vector &v) { return Vector(n * v.x, n * v.y, n * v.z);}
        friend std::ostream & operator<< (std::ostream &out, const Vector &v) { return out << v.x << ", " << v.y << ", " << v.z;}
};

class RGB: public Vector {
    public:
        float r;
        float g;
        float b;

        RGB(): Vector(), r(x), g(y), b(z) {}
        RGB(const float &n): Vector(n), r(x), g(y), b(z) {}
        RGB(const float &tr, const float &tg, const float &tb): Vector(tr, tg, tb), r(x), g(y), b(z) {}
        RGB(const Vector &v): Vector(v), r(x), g(y), b(z) {}

        void operator= (const Vector &v) { x = v.x; r = v.x; y = v.y; g = v.y; z = v.z; b = v.z;}

        friend std::ostream & operator<< (std::ostream &out, const RGB &v) { return out << v.r << " " << v.g << " " << v.b;}
};


#endif