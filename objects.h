#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include <iostream>
#include "vectors.h"

enum SurfaceType { OPAQUE, MIRROR, TRANSPARENT};

class Object {
    public:
        RGB color;
        Point position;
        SurfaceType surfaceType;

        Object() {}

};

class Sphere: public Object {
    public:
        float radius;

        Sphere (const float &f = 0.f): Object(), radius(f) {}
        Sphere (const Sphere &s): Object(), radius(s.radius) {}

        void operator= (const Sphere &s) { radius = s.radius;}
};

class Parallelepiped: public Object {
    public:
        float a;
        float b;
        float c;

        Parallelepiped (const float &n = 0.f): Object(), a(n), b(n), c(n) {}
        Parallelepiped (const float &ta, const float &tb, const float &tc): Object(), a(ta), b(tb), c(tc) {}
        Parallelepiped (const Parallelepiped &p): Object(), a(p.a), b(p.b), c(p.c) {}

        void operator= (const Parallelepiped &p) { a = p.a; b = p.b; c = p.c;}
};

#endif