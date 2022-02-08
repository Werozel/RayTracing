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
    Vector *norm{};

    Object(const Point &pos, const Material &m);

    [[nodiscard]] Point &get_position() const;

    [[nodiscard]] Material &get_material() const;

    [[nodiscard]] virtual RGB get_color(const Point &p) const;

    [[nodiscard]] SurfaceType get_stype() const;

    [[nodiscard]] float get_deffuse_coef() const;

    [[nodiscard]] float get_mirror_coef() const;

    [[nodiscard]] float get_shininess() const;

    [[nodiscard]] float get_refractive_index() const;

    // Returns an intersection point of ray and current object
    [[nodiscard]] virtual Point ray_intersection(const Ray &ray) const;

    // Returns a normal vector through the point on object
    [[nodiscard]] virtual Vector get_norm(const Point &p) const;

    ~Object();
};

class Sphere : public Object {
public:
    float radius;

    Sphere(const float &rad,
           const Point &pos, const Material &m) :
            Object(pos, m), radius(rad) {}

    Sphere(const Sphere &s);

    [[nodiscard]] float get_radius() const;

    Sphere &operator=(const Sphere &s);

    [[nodiscard]] Point ray_intersection(const Ray &ray) const override;

    // Vector from center to p
    [[nodiscard]] Vector get_norm(const Point &p) const override;

    ~Sphere();
};


class Polygon : public Object {
public:
    Point *p1;
    Point *p2;
    Point *p3;

    Polygon(const Point &pos, const Material &m,
            const Point &p1, const Point &p2, const Point &p3);

    Polygon(const Polygon &p);

    [[nodiscard]] Point &get_p1() const;

    [[nodiscard]] Point &get_p2() const;

    [[nodiscard]] Point &get_p3() const;

    [[nodiscard]] Vector &get_polygon_norm() const;

    Polygon &operator=(const Polygon &p);

    [[nodiscard]] Point ray_intersection(const Ray &ray) const override;

    [[nodiscard]] Vector get_norm(const Point &p) const override;

    ~Polygon();
};


class Rectangle : public Object {
public:
    Polygon p1;
    Polygon p2;

    Rectangle(const Point &pos,
              const Material &m,
              const Point &p1_t,
              const Point &p2_t,
              const Point &p3_t,
              const Point p4_t);

    Rectangle(const Rectangle &r);

    Rectangle& operator=(const Rectangle &r);

    [[nodiscard]] Point ray_intersection(const Ray &ray) const override;

    [[nodiscard]] Vector get_norm(const Point &p) const override;
};


class SceneFloor : public Object {
public:
    RGB *second_color;
    float tile_size;

    SceneFloor(const Point &pos, const Material &m, const Colors &s_col = BG_COLOR, const float &pal_size = 75);

    SceneFloor(const SceneFloor &f);

    [[nodiscard]] RGB get_second_color() const;

    [[nodiscard]] float get_tile_size() const;

    SceneFloor& operator=(const SceneFloor &f);

    [[nodiscard]] Point ray_intersection(const Ray &ray) const override;

    [[nodiscard]] Vector get_norm(const Point &p) const override;

    [[nodiscard]] RGB get_color(const Point &p) const override;

    ~SceneFloor();
};


class Ray {
public:
    Point *start;
    Vector *direction;

    Ray();

    Ray(const Point &start_point, const Vector &dir);

    [[nodiscard]] Point &get_start() const;

    [[nodiscard]] Vector &get_direction() const;


    // Gets a point on a ray closest to given point
    [[nodiscard]] Point get_closest_point_to_point(const Point &p) const;

    // Gets a point on a ray closest to given object
    [[nodiscard]] Point get_closest_point_to_object(const Object &o) const;

    Ray& operator=(const Ray &r);

    ~Ray();
};

#endif