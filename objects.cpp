#include "objects.h"
#include "vectors.h"


Object::Object(const RGB &col, const Point &pos, const SurfaceType &stype, const float &shine, const float &refr_index):
     surfaceType(stype), shininess(shine), refractiveIndex(refr_index) {color = new RGB(col); position = new Point(pos);}

RGB Object::get_color() const { return *color;}
Point Object::get_position() const { return *position;}
SurfaceType Object::get_stype() const { return surfaceType;}
float Object::get_shininess() const { return shininess;}
float Object::get_refractive_index() const { return refractiveIndex;}

Object::~Object () {delete color; delete position;}

Point Object::ray_intersection(const Ray &ray) const {
    return Point(0, 0, 0);
}


// --------------------- Sphere --------------------------
Sphere::Sphere (const Sphere &s): Object(s.get_color(), s.get_position(), s.get_stype(), s.get_shininess(), s.get_refractive_index()), radius(s.get_radius()) {}
void Sphere::operator= (const Sphere &s) {
    color = new RGB(s.get_color()); 
    position = new Point(s.get_position());
    surfaceType = s.get_stype();
    refractiveIndex = s.get_refractive_index();
    radius = s.get_radius();
}

float Sphere::get_radius () const { return radius;}

Point Sphere::ray_intersection(const Ray &ray) const {
    Point p = ray.get_closest_point_to_object(*this);
    float dist = distance(p, *position);
    bool is_inside = distance(*ray.start, *position) <= radius;
    if (dist <= radius) {
        Vector v = Vector(p, *ray.start).normalize();
        if (is_inside) v = -v;
        float half_horde = sqrt(radius * radius - dist * dist);
        return p + v * half_horde;
    } else {
        return Point(-1, -1, -1);
    }
}


// --------------------- Parallelepiped -----------------------
Parallelepiped::Parallelepiped (const Parallelepiped &p): 
    Object(p.get_color(), p.get_position(), p.get_stype(), p.get_shininess()), a(p.get_a()), b(p.get_b()), c(p.get_c()) {}
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
        v = -v;
    } 
    Point t = *start;
    float k = (v * direction->normalize())/ direction->get_length();
    Vector shift = k * direction->normalize();
    t = t + shift;
    return t;
}

Point Ray::get_closest_point_to_object (const Object &o) const {
    return get_closest_point_to_point(o.get_position());
}

Ray::~Ray () {
    delete start;
    delete direction;
}
