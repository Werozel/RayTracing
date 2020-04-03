#ifndef MATERIALS_H
#define MATERIALS_H


#include "constants.h"

class RGB;


class Material {
    public:
        RGB* color;
        float deffuse_coef;
        float mirror_coef;
        float shininess;
        float refractive_index;
        SurfaceType surface_type;

        Material(const SurfaceType &stype,
                 const Colors &col = WHITE, const float &deffuse_c = 1, 
                 const float &mirror_c = 1, const float &shine = 70, 
                 const float &refr_index = 1);
        Material(const Material &m);

        RGB & get_obj_color() const;
        float get_deffuse_coef() const;
        float get_mirror_coef() const;
        float get_shininess() const;
        float get_refractive_index() const;
        SurfaceType get_surface_type() const;

        void operator= (const Material &m);

        ~Material();
};

RGB get_material_color(const Colors &color = BG_COLOR);
Material get_material(const Materials &m, const Colors &color = BLACK);


#endif