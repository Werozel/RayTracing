#include "objects.h"
#include "vectors.h"


Object::Object(const RGB &col, const Point &pos, const SurfaceType &stype): surfaceType(stype) {color = new RGB(col); position = new Point(pos);}

RGB Object::get_color() const { return *color;}
Point Object::get_position() const { return *position;}
SurfaceType Object::get_stype() const { return surfaceType;}

Object::~Object () {delete color; delete position;}

Point Object::ray_intersection(const Ray &ray) const {
    return Point(0, 0, 0);
}


// --------------------- Sphere --------------------------
Sphere::Sphere (const Sphere &s): Object(s.get_color(), s.get_position(), s.get_stype()), radius(s.get_radius()) {}
void Sphere::operator= (const Sphere &s) {
    color = new RGB(s.get_color()); 
    position = new Point(s.get_position());
    surfaceType = s.get_stype();
    radius = s.get_radius();
}

float Sphere::get_radius () const { return radius;}

Point Sphere::ray_intersection(const Ray &ray) const {
    Point p = ray.get_closest_point_to_object(*this);
    float dist = distance(p, *position);
    if (dist <= radius) {
        Vector v = Vector(p, *ray.start).normalize();
        float half_horde = sqrt(radius * radius - dist * dist);
        return p + v * half_horde;
    } else {
        return Point(-1, -1, -1);
    }
}


// --------------------- Parallelepiped -----------------------
Parallelepiped::Parallelepiped (const Parallelepiped &p): 
    Object(p.get_color(), p.get_position(), p.get_stype()), a(p.get_a()), b(p.get_b()), c(p.get_c()) {}
void Parallelepiped::operator= (const Parallelepiped &p) { 
    color = new RGB(p.get_color()); 
    position = new Point(p.get_position());
    surfaceType = p.get_stype();
    a = p.get_a(); b = p.get_b(); c = p.get_c();
}
float Parallelepiped::get_a() const { return a;}
float Parallelepiped::get_b() const{ return b;}
float Parallelepiped::get_c() const { return c;}


// ----------------------- Ray -------------------------------
Ray::Ray(const Point &start_point, const Vector &dir) {start = new Point(start_point); direction = new Vector(dir);}

Point Ray::get_closest_point_to_point(const Point &p) const {
    Vector v = Vector(*start, p);
    float cos = v * *direction / v.get_length() / direction->get_length();
    if (cos <= 0) {
        return *start;
    } else {
        Point t = *start;
        float k = (v * direction->normalize())/ direction->get_length();
        Vector shift = k * direction->normalize();
        t = t + shift;
        return t;
    }
}

Point Ray::get_closest_point_to_object (const Object &o) const {
    return get_closest_point_to_point(o.get_position());
}

Ray::~Ray () {
    delete start;
    delete direction;
}
