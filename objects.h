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
        float shininess;

        Object(const RGB &col, const Point &pos, const SurfaceType &stype, const float &shine = 60);

        RGB get_color() const;
        Point get_position() const;
        SurfaceType get_stype() const;
        float get_shininess() const;

        virtual Point ray_intersection(const Ray &ray) const;

        virtual ~Object ();
};

class Sphere: public Object {
    public:
        float radius;

        Sphere (const float &rad, const RGB &col, 
                const Point &pos, const SurfaceType &surf_type, const float &shine = 60): 
            Object(col, pos, surf_type, shine), radius(rad) {}
        Sphere (const Sphere &s);

        float get_radius() const;

        void operator= (const Sphere &s);

        virtual Point ray_intersection(const Ray &ray) const;
};

class Parallelepiped: public Object {
    public:
        float a;
        float b;
        float c;

        Parallelepiped (const float &n, const RGB &col, 
                        const Point &pos, const SurfaceType &surf_type, const float &shine = 60): 
            Object(col, pos, surf_type, shine), a(n), b(n), c(n) {}
        Parallelepiped (const float &ta, const float &tb, const float &tc, 
                        const RGB &col, const Point &pos, const SurfaceType &surf_type, const float &shine): 
            Object(col, pos, surf_type, shine), a(ta), b(tb), c(tc) {}
        Parallelepiped (const Parallelepiped &p);

        virtual Point ray_intesection(const Ray &ray) const = 0;
        
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

        Point get_closest_point_to_point (const Point &p) const;

        Point get_closest_point_to_object (const Object &o) const;

        ~Ray ();
        
};

#endif