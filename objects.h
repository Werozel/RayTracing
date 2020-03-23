#ifndef OBJECTS_H
#define OBJECTS_H

#include <iostream>


class Sphere;
class Parallelepiped;
class Point;
class Vector;
class Triplet;
class RGB;
class Ray;


enum SurfaceType { OPAQUE, MIRROR, TRANSPARENT};
enum IntersectionType { ABSORPTION, REFLECTION, REFLECTION_AND_REFRACTION};

class Object {
    public:
        RGB *color;
        Point *position;
        SurfaceType surfaceType;

        Object(const RGB &col, const Point &pos, const SurfaceType &stype);

        RGB get_color() const;
        Point get_position() const;
        SurfaceType get_stype() const;

        bool ray_intersection(const Ray &ray) const;

        ~Object ();
};

class Sphere: public Object {
    public:
        float radius;

        Sphere (const float &f, const RGB &col, 
                const Point &pos, const SurfaceType &surf_type): 
            Object(col, pos, surf_type), radius(f) {}
        Sphere (const Sphere &s);

        float get_radius() const;

        void operator= (const Sphere &s);

        bool ray_intesection(const Ray &ray) const;
};

class Parallelepiped: public Object {
    public:
        float a;
        float b;
        float c;

        Parallelepiped (const float &n, const RGB &col, 
                        const Point &pos, const SurfaceType &surf_type): 
            Object(col, pos, surf_type), a(n), b(n), c(n) {}
        Parallelepiped (const float &ta, const float &tb, const float &tc, 
                        const RGB &col, const Point &pos, const SurfaceType &surf_type): 
            Object(col, pos, surf_type), a(ta), b(tb), c(tc) {}
        Parallelepiped (const Parallelepiped &p);
        
        float get_a() const;
        float get_b() const;
        float get_c() const;

        void operator= (const Parallelepiped &p);
};


class Ray {
    public:
        Point *start;
        Vector *direction;

        Ray(const Point &start_point, const Vector &dir);

        float distance_to_point (const Point &p) const;

        float distance_to_object (const Object &o) const;

        ~Ray ();
        
};

#endif