#ifndef MATERIALS_H
#define MATERIALS_H


#include "constants.h"

class RGB;


class Material {
public:
    static RGB *bg_color;
    RGB *color;
    float deffuse_coef;
    float mirror_coef;
    float shininess;
    float refractive_index;
    SurfaceType surface_type;

    explicit Material(const SurfaceType &stype,
                      const Colors &col = BLACK,
                      const float &deffuse_c = 1,
                      const float &mirror_c = 1,
                      const float &shine = 70,
                      const float &refr_index = 1);

    Material(const Material &m);

    [[nodiscard]] RGB &get_obj_color() const;

    [[nodiscard]] float get_deffuse_coef() const;

    [[nodiscard]] float get_mirror_coef() const;

    [[nodiscard]] float get_shininess() const;

    [[nodiscard]] float get_refractive_index() const;

    [[nodiscard]] SurfaceType get_surface_type() const;

    Material& operator=(const Material &m);

    ~Material();
};

RGB get_material_color(const Colors &color = BG_COLOR);

Material get_material(const Materials &m, const Colors &color = BLACK);


#endif