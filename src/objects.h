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

        // Returns a normal vector through the point on object
        virtual Vector get_norm(const Point &p) const;

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

        virtual Point ray_intersection(const Ray &ray) const;
        // Vector from center to p
        virtual Vector get_norm(const Point &p) const;

};


class Polygon: public Object {
    public:
        // In relation to position
        Vector *v1;
        Vector *v2;
        Vector *v3;

        Polygon (const Point &pos, const Material &m, const Vector &p1, const Vector &p2, const Vector &p3);
        Polygon (const Polygon &p);

        Vector get_v1() const;
        Vector get_v2() const;
        Vector get_v3() const;

        void operator= (const Polygon &p);

        virtual Point ray_intersection(const Ray &ray) const;
        virtual Vector get_norm(const Point &p) const;

        ~Polygon();
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