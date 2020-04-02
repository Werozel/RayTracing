#include "objects.h"
#include "vectors.h"
#include "materials.h"


Object::Object(const Point &pos, const Material &m)
    {position = new Point(pos);  material = new Material(m);}

Point & Object::get_position() const { return *position;}
Material & Object::get_material() const { return *material;}
RGB Object::get_color() const { return material->get_obj_color();}
SurfaceType Object::get_stype() const { return material->get_surface_type();}
float Object::get_deffuse_coef() const { return material->get_deffuse_coef();}
float Object::get_mirror_coef() const { return material->get_mirror_coef();}
float Object::get_shininess() const { return material->get_shininess();}
float Object::get_refractive_index() const { return material->get_refractive_index();}

Object::~Object () {delete position; delete material;}

Point Object::ray_intersection(const Ray &ray) const {
    return Point(0, 0, 0);
}

Vector Object::get_norm(const Point &p) const {
    return Vector(0, 0, 0);
}



// --------------------- Sphere --------------------------
Sphere::Sphere (const Sphere &s): 
    Object(s.get_position(), s.get_material()), 
    radius(s.get_radius()) {}

void Sphere::operator= (const Sphere &s) {
    if (&s == this) { return;}
    delete position;
    delete material;
    position = new Point(s.get_position());
    material = new Material(s.get_material());
    radius = s.get_radius();
}

float Sphere::get_radius () const { return radius;}

Point Sphere::ray_intersection(const Ray &ray) const {
    Point p = ray.get_closest_point_to_object(*this);
    float dist = distance(p, *position);
    bool is_inside = distance(ray.get_start(), *position) <= radius;
    if (dist <= radius) {
        Vector v = Vector(p, *ray.start).normalize();
        if (is_inside) v = -v;
        float half_horde = sqrt(radius * radius - dist * dist);
        return p + v * half_horde;
    } else {
        return Point(-1, -1, -1);
    }
}

Vector Sphere::get_norm(const Point &p) const {
    return Vector(*position, p).normalize();
}


// ----------------------- Polygon ---------------------------
Polygon::Polygon (const Point &pos, const Material &m, const Point &p1_t, const Point &p2_t, const Point &p3_t):
    Object(pos, m), p1(new Point(p1_t)), p2(new Point (p2_t)), p3(new Point(p3_t)) {}

Polygon::Polygon (const Polygon &p): Object(p.get_position(), p.get_material()), 
    p1(new Point(p.get_p1())), p2(new Point(p.get_p2())), p3(new Point(p.get_p3())) {}


Vector Polygon::get_v1() const { return Vector(*position, *p1);}
Vector Polygon::get_v2() const { return Vector(*position, *p2);}
Vector Polygon::get_v3() const { return Vector(*position, *p3);}
Point & Polygon::get_p1() const { return *p1;}
Point & Polygon::get_p2() const { return *p2;}
Point & Polygon::get_p3() const { return *p3;}


void Polygon::operator= (const Polygon &p) {
    if (&p == this) { return;}
    delete position;
    delete material;
    delete p1;
    delete p2;
    delete p3;
    position = new Point(p.get_position());
    material = new Material(p.get_material());
    p1 = new Point(p.get_p1());
    p2 = new Point(p.get_p2());
    p3 = new Point(p.get_p3());
}

Point Polygon::ray_intersection(const Ray &ray) const {
    // Checks if current ray direction is towards the object
    Vector E1(*p1, *p2), E2(*p1, *p3);
    Vector T(*p1, ray.get_start());
    Vector P(cross_prod(ray.get_direction(), E2));
    Vector Q(cross_prod(T, E1));

    float z = P * E1;
    float t = Q * E2 / z;
    float u = P * T / z;
    float v = Q * ray.get_direction() / z;
    float k = 1 - u - v;
    
    if (k >= 0 && u >= 0 && v >= 0 && t > 0) {
        // return Point(k * *p2 + u * *p3 + v * *p1);
        return Point(ray.get_start() + t * ray.get_direction());
    } else {
        return Point(-1, -1, -1);
    }
}

Vector Polygon::get_norm(const Point &p) const {
    // TODO figure out the direction of norm
    return cross_prod(Vector(p, *p1), Vector(p, *p2)).normalize();
}

Polygon::~Polygon() {
    delete p1;
    delete p2;
    delete p3;
}

// ----------------------- Ray -------------------------------
Ray::Ray(): start(nullptr), direction(nullptr) {}
Ray::Ray(const Point &start_point, const Vector &dir) 
    {start = new Point(start_point); direction = new Vector(dir.normalize());}

Point & Ray::get_start() const { return *start;}
Vector & Ray::get_direction() const { return *direction;}

void Ray::operator= (const Ray &r) {
    if (&r == this) { return;}
    delete start;
    delete direction;
    start = new Point(r.get_start());
    direction = new Vector(r.get_direction());
}

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
