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
class Material;


#include "constants.h"


class Object {
    public:
        Point *position;
        Material *material;

        Object(const Point &pos, const Material &m);

        Point get_position() const;
        Material get_material() const;
        RGB get_color() const;
        SurfaceType get_stype() const;
        float get_deffuse_coef() const;
        float get_mirror_coef() const;
        float get_shininess() const;
        float get_refractive_index() const;
        
        // Returns an intersection point of ray and current object
        virtual Point ray_intersection(const Ray &ray) const;

        virtual ~Object ();
};

class Sphere: public Object {
    public:
        float radius;

        Sphere (const float &rad,
                const Point &pos, const Material &m): 
            Object(pos, m), radius(rad) {}
        Sphere (const Sphere &s);

        float get_radius() const;

        void operator= (const Sphere &s);

        // Returns an intersection point of ray and current object
        virtual Point ray_intersection(const Ray &ray) const;
};


class Ray {
    public:
        Point *start;
        Vector *direction;

        Ray(const Point &start_point, const Vector &dir);

        // Gets a point on a ray closest to given point
        Point get_closest_point_to_point (const Point &p) const;

        // Gets a point on a ray closest to given object
        Point get_closest_point_to_object (const Object &o) const;

        ~Ray ();
};

#endif