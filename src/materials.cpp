#include "materials.h"
#include "vectors.h"


Material::Material(const SurfaceType &stype,
                   const Colors &col, const float &deffuse_c,
                   const float &mirror_c, const float &shine,
                   const float &refr_index) :
        surface_type(stype), deffuse_coef(deffuse_c), mirror_coef(mirror_c),
        shininess(shine), refractive_index(refr_index) {
    color = new RGB(get_material_color(col));
}


Material::Material(const Material &m) :
        surface_type(m.get_surface_type()), deffuse_coef(m.get_deffuse_coef()),
        mirror_coef(m.get_mirror_coef()), shininess(m.get_shininess()),
        refractive_index(m.get_refractive_index()) {
    color = new RGB(m.get_obj_color());
}


RGB &Material::get_obj_color() const { return *color; }

float Material::get_deffuse_coef() const { return deffuse_coef; }

float Material::get_mirror_coef() const { return mirror_coef; }

float Material::get_shininess() const { return shininess; }

float Material::get_refractive_index() const { return refractive_index; }

SurfaceType Material::get_surface_type() const { return surface_type; }

Material& Material::operator=(const Material &m) {
    if (&m == this) { return *this; }
    surface_type = m.get_surface_type();
    delete color;
    color = new RGB(m.get_obj_color());
    deffuse_coef = m.get_deffuse_coef();
    mirror_coef = m.get_mirror_coef();
    shininess = m.get_shininess();
    refractive_index = m.get_refractive_index();
    return *this;
}

Material::~Material() { delete color; }


Material get_material(const Materials &m, const Colors &color) {
    switch (m) {
        case GLASS:
            return Material(TRANSPARENT, BLACK, 1, 1.5, 250, 1.43);
        case METAL:
            return Material(MIRROR, BLACK, 0.2, 1.5, 400, 1);
        case PLASTIC:
            return Material(OPAQUE, color, 0.7, 1, 110, 1);
        case COLORED_METAL:
            return Material(MIRROR, color, 0.6, 1, 400, 1);
        case MARBLE:
            return Material(OPAQUE, WHITE, 1, 1, 110, 1);
        default:
            return Material(OPAQUE);
    }
}

RGB get_material_color(const Colors &color) {
    switch (color) {
        case WHITE:
            return RGB {255, 255, 255 };
        case BLACK:
            return RGB {0, 0, 0 };
        case GRAY:
            return RGB {220, 220, 220 };
        case RED:
            return RGB {255, 0, 0 };
        case GREEN:
            return RGB {0, 255, 0 };
        case BLUE:
            return RGB {0, 0, 255 };
        case LIGHT_BLUE:
            return RGB {103, 213, 213 };
        case PURPLE:
            return RGB {162, 1, 202 };
        case DARK_PURPLE:
            return RGB {159, 43, 104 };
        case YELLOW:
            return RGB {245, 230, 31 };
        case ORANGE:
            return RGB {236, 155, 50 };
        case BROWN:
            return RGB {152, 110, 85 };
        case PINK:
            return RGB {226, 114, 178 };
        case DARK_PINK:
            return RGB {171, 66, 125 };
        default: //BG_COLOR
            return *Material::bg_color;
    }
}
