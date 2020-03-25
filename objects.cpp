#include "objects.h"
#include "vectors.h"


Object::Object(const RGB &col, const Point &pos, const SurfaceType &stype): surfaceType(stype) {color = new RGB(col); position = new Point(pos);}

RGB Object::get_color() const { return *color;}
Point Object::get_position() const { return *position;}
SurfaceType Object::get_stype() const { return surfaceType;}

Object::~Object () {delete color; delete position;}

bool Object::ray_intersection(const Ray &ray) const {
    return false;
}


Sphere::Sphere (const Sphere &s): Object(s.get_color(), s.get_position(), s.get_stype()), radius(s.get_radius()) {}
void Sphere::operator= (const Sphere &s) {
    color = new RGB(s.get_color()); 
    position = new Point(s.get_position());
    surfaceType = s.get_stype();
    radius = s.get_radius();
}

float Sphere::get_radius () const { return radius;}

bool Sphere::ray_intersection(const Ray &ray) const {
    float dist = ray.distance_to_object(*this);
    return dist <= radius;
}


Parallelepiped::Parallelepiped (const Parallelepiped &p): 
    Object(p.get_color(), p.get_position(), p.get_stype()), a(p.get_a()), b(p.get_b()), c(p.get_c()) {}
void Parallelepiped::operator= (const Parallelepiped &p) { a = p.get_a(); b = p.get_b(); c = p.get_c();}
float Parallelepiped::get_a() const { return a;}
float Parallelepiped::get_b() const{ return b;}
float Parallelepiped::get_c() const { return c;}



Ray::Ray(const Point &start_point, const Vector &dir) {start = new Point(start_point); direction = new Vector(dir);}

float Ray::distance_to_point (const Point &p) const {
    Vector v = Vector(*start, p);
    float cos = v * *direction / v.get_length() / direction->get_length();
    if (cos <= 0) {
        return distance(*start, p);
    } else {
        Point t = *start;
        float k = (v * direction->normalize())/ direction->get_length();
        Vector shift = k * direction->normalize();
        t = t + shift;
        return distance(p, t);
    }
}

float Ray::distance_to_object (const Object &o) const {
    return distance_to_point(o.get_position());
}

Ray::~Ray () {
    delete start;
    delete direction;
}
