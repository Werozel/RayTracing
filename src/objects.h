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

        Point & get_position() const;
        Material & get_material() const;
        virtual RGB get_color(const Point &p) const;
        SurfaceType get_stype() const;
        float get_deffuse_coef() const;
        float get_mirror_coef() const;
        float get_shininess() const;
        float get_refractive_index() const;
        
        // Returns an intersection point of ray and current object
        virtual Point ray_intersection(const Ray &ray) const;

        // Returns a normal vector through the point on object
        virtual Vector get_norm(const Point &p) const;

        ~Object ();
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

        Point ray_intersection(const Ray &ray) const;
        // Vector from center to p
        Vector get_norm(const Point &p) const;

        ~Sphere();
};


class Polygon: public Object {
    public:
        Point *p1;
        Point *p2;
        Point *p3;
        Vector *norm;

        Polygon (const Point &pos, const Material &m, const Point &p1, const Point &p2, const Point &p3);
        Polygon (const Polygon &p);

        // From position to pi
        Vector get_v1() const;
        Vector get_v2() const;
        Vector get_v3() const;
        Point & get_p1() const;
        Point & get_p2() const;
        Point & get_p3() const;
        Vector & get_polygon_norm() const;

        void operator= (const Polygon &p);

        Point ray_intersection(const Ray &ray) const;
        Vector get_norm(const Point &p) const;

        ~Polygon();
};


class Rectangle: public Object {
    public:
        Polygon p1;
        Polygon p2;

        Rectangle(const Point &pos, const Material &m, const Point &p1_t, const Point &p2_t, const Point &p3_t, const Point p4_t);
        Rectangle(const Rectangle &r);

        void operator= (const Rectangle &r);

        Point ray_intersection(const Ray &ray) const;
        Vector get_norm(const Point &p) const;
};


class SceneFloor: public Object {
    public:
        RGB *second_color;
        float tile_size;

        SceneFloor(const Point &pos, const Material &m, const Colors &s_col = BG_COLOR, const float &pal_size = 75);
        SceneFloor(const SceneFloor &f);

        RGB get_second_color() const;
        float get_tile_size() const;

        void operator= (const SceneFloor &f);

        Point ray_intersection(const Ray &ray) const;
        Vector get_norm(const Point &p) const;

        RGB get_color(const Point &p) const;

        ~SceneFloor();
};


class Ray {
    public:
        Point *start;
        Vector *direction;

        Ray();
        Ray(const Point &start_point, const Vector &dir);

        Point & get_start() const;
        Vector & get_direction() const; 
        

        // Gets a point on a ray closest to given point
        Point get_closest_point_to_point (const Point &p) const;

        // Gets a point on a ray closest to given object
        Point get_closest_point_to_object (const Object &o) const;

        void operator= (const Ray &r);

        ~Ray ();
};

#endif