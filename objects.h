#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include <iostream>

class Triplet;
class RGB;
class Point;
class Vector;
class Ray;


enum SurfaceType { OPAQUE, MIRROR, TRANSPARENT};
enum IntersectionType { ABSORPTION, REFLECTION, REFLECTION_AND_REFRACTION};

class Object {
    public:
        RGB color;
        Point position;
        SurfaceType surfaceType;

        Object(const RGB &col, const Point &pos, const SurfaceType &stype): color(col), position(pos), surfaceType(stype) {}

        bool ray_intersection(const Ray &ray) const {
            return false;
        }

};

class Sphere: public Object {
    public:
        float radius;

        Sphere (const float &f, const RGB &col, const Point &pos, const SurfaceType &surf_type): Object(col, pos, surf_type), radius(f) {}
        Sphere (const Sphere &s): Object(s.color, s.position, s.surfaceType), radius(s.radius) {}

        void operator= (const Sphere &s) { radius = s.radius;}

        bool ray_intesection(const Ray &ray) const{
            float dist = ray.distance_to_object(*this);
            return dist <= radius;
        }
};

class Parallelepiped: public Object {
    public:
        float a;
        float b;
        float c;

        Parallelepiped (const float &n, const RGB &col, const Point &pos, const SurfaceType &surf_type): Object(col, pos, surf_type), a(n), b(n), c(n) {}
        Parallelepiped (const float &ta, const float &tb, const float &tc, const RGB &col, const Point &pos, const SurfaceType &surf_type): 
            Object(col, pos, surf_type), a(ta), b(tb), c(tc) {}
        Parallelepiped (const Parallelepiped &p): Object(p.color, p.position, p.surfaceType), a(p.a), b(p.b), c(p.c) {}

        void operator= (const Parallelepiped &p) { a = p.a; b = p.b; c = p.c;}
};


class Ray {
    public:
        Point start;
        Vector direction;

        Ray(const Point &start_point, const Vector &dir): start(start_point), direction(dir) {}

        float distance_to_point (const Point &p) const {
            Vector v = Vector(start, p).normalize();
            float cos = v * direction;
            if (cos <= 0) {
                return distance(start, p);
            } else {
                return distance(p, start + (v * direction)/direction.get_length() * direction.normalize());
            }
        }

        float distance_to_object (const Object &o) const {
            return distance_to_point(o.position);
        }
        
};

#endif