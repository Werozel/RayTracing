#include "materials.h"
#include "vectors.h"
#include "objects.h"


Material::Material(const SurfaceType &stype,
                    const Colors &col, const float &deffuse_c, 
                    const float &mirror_c, const float &shine, 
                    const float &refr_index):
        surface_type(stype), color(new RGB(get_color(col))),
        deffuse_coef(deffuse_c), mirror_coef(mirror_c),
        shininess(shine), refractive_index(refr_index)  {}


Material::Material(const Material &m): 
    surface_type(m.get_surface_type()),
    color(new RGB(m.get_obj_color())), deffuse_coef(m.get_deffuse_coef()),
    mirror_coef(m.get_mirror_coef()), shininess(m.get_shininess()), 
    refractive_index(m.get_refractive_index()) {}


RGB & Material::get_obj_color() const { return *color;}
float Material::get_deffuse_coef() const { return deffuse_coef;}
float Material::get_mirror_coef() const { return mirror_coef;}
float Material::get_shininess() const { return shininess;}
float Material::get_refractive_index() const { return refractive_index;}
SurfaceType Material::get_surface_type() const { return surface_type;}

void Material::operator=(const Material &m) {
    if (&m == this) { return;}
    surface_type = m.get_surface_type();
    delete color;
    color = new RGB(m.get_obj_color());
    deffuse_coef = m.get_deffuse_coef();
    mirror_coef = m.get_mirror_coef();
    shininess = m.get_shininess();
    refractive_index = m.get_refractive_index();
}

Material::~Material() { delete color;}


Material get_material(const Materials &m, const Colors &color) {
    switch (m) {
        case GLASS:
            return Material(TRANSPARENT, color, 1, 1.7, 250, 1.5);
        case METAL:
            return Material(MIRROR, color, 1, 1.5, 400, 1);
        case PLASTIC:
            return Material(OPAQUE, color, 0.9, 1, 110, 1);
        default:
            return Material(OPAQUE);
    }
}

RGB get_color (const Colors &color) {
        switch (color) {
        case WHITE:
            return RGB(255, 255, 255);
        case BLACK:
            return RGB(0, 0, 0);
        case GRAY:
            return RGB(220, 220, 220);
        case RED:
            return RGB(255, 0, 0);
        case GREEN:
            return RGB(0, 255, 0);
        case BLUE:
            return RGB(0, 0, 255);
        case PURPLE:
            return RGB(162, 1, 202);
        default: //BG_COLOR
            return RGB(103, 213, 213);
    }
}
