#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include <iostream>
#include "vectors.h"
#include "vectors.cpp"

enum SurfaceType { OPAQUE, MIRROR, TRANSPARENT};
enum IntersectionType { ABSORPTION, REFLECTION, REFLECTION_AND_REFRACTION};

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


class Ray {
    public:
        Point start;
        Vector direction;

    float distance_to_point (const Point &p) {
        Vector v = Vector(start, p).normalize();
        float cos = v * direction;
        if (cos <= 0) {
            return distance(start, p);
        } else {
            return distance(p, start + (v * direction)/direction.get_length() * direction.normalize());
        }
    }

    float distance_to_object (const Object &o) {
        return distance_to_point(o.position);
    }
        
};

#endif