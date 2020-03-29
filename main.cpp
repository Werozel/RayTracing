#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <chrono>
#include <algorithm>
#include <omp.h>
#include "src/constants.h"
#include "src/vectors.h"
#include "src/objects.h"
#include "src/materials.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"


RGB cast_ray(const Ray &ray, const std::vector<Sphere> &objects, 
             const std::vector<Light> &lights, const int &depth = 0) {
    if (objects.size() == 0) return get_color();
    if (depth == recursion_gap) return get_color();

    Point no_intersection(-1, -1, -1);
    Point intersection_point = no_intersection;
    float min_dist = std::numeric_limits<float>::max();
    Sphere intersected_obj(objects[0]);
    int obj_i = 0;
    for (int i = 0; i < objects.size(); i++) {
        // Returns Point(-1, -1, -1) if no intersection detected
        Point curr_intersection = objects[i].ray_intersection(ray); // Intersection point of ray
        if (curr_intersection != no_intersection) {
            float curr_distance = distance(curr_intersection, *ray.start);
            if (curr_distance < min_dist) {
                min_dist = curr_distance;
                intersection_point = curr_intersection;
                intersected_obj = objects[i];
                obj_i = i;
            }
        }
    }

    if (intersection_point == no_intersection)  { return get_color();}

    Vector norm = intersected_obj.get_norm(intersection_point);
    
    // Calcu;ating brightness
    float brightness = 0; // Brightnesss of intersection point
    for (auto light: lights) {
        Vector vector_of_incidence(intersection_point, light.position); // To light direction
        
        Ray to_light(intersection_point, vector_of_incidence.normalize());
        bool shade_flag = false; // Flag indicating if point is in shade for current light
        for (int j = 0; j < objects.size(); j++) {
            if (j == obj_i) continue; // Skipping current object
            if (objects[j].ray_intersection(to_light) != no_intersection) {
                shade_flag = true;
                break;
            }
        }
        if (shade_flag) continue; // If in shade skipping brightness calculation

        float angle_of_incidence = get_angle(norm, vector_of_incidence);
        if (angle_of_incidence > 0) {
            // Calculating deffuse brightness
            if (intersected_obj.get_stype() == OPAQUE ) brightness += angle_of_incidence * light.intensity * intersected_obj.get_deffuse_coef();
            
            // Calculating glare brightness
            // Direction to camera from intersection point
            Vector to_camera = 2 * (vector_of_incidence * norm) * norm - vector_of_incidence;
            float angle_of_reflection = get_angle(norm, to_camera);
            // Calculating a glare ---  K * (n * to_camera)^p
            brightness += light.intensity * intersected_obj.get_mirror_coef() * std::pow(angle_of_reflection, intersected_obj.get_shininess());
        }
    }

    
    // Getting the color of the point
    RGB result = get_color();
    if (intersected_obj.get_stype() == OPAQUE) {
        result = intersected_obj.get_color() * brightness;

    } else {       // if MIRROR or
        Vector reflect_dir = *ray.direction - 2 * (*ray.direction * norm) * norm;
        Ray reflected_ray(intersection_point, reflect_dir.normalize());
        RGB reflection_result = cast_ray(reflected_ray, objects, lights, depth + 1);

        RGB refraction_result(0, 0, 0);

        if (intersected_obj.get_stype() == TRANSPARENT) {
            reflection_result = reflection_result * 0.2;        

            Vector tmp_norm = norm;  // For calculating an angle between norm and ray direction
            float angle_of_incidence = get_angle(-tmp_norm, *ray.direction); // Angle of incidence of current ray
            float n1 = 1; 
            float n2 = intersected_obj.get_refractive_index();
            if (angle_of_incidence < 0) {   // If ray is inside the object reverse the layout
                std::swap(n1, n2);
                angle_of_incidence *= -1;
                tmp_norm = -tmp_norm;
            }
            float k = n1 / n2;
            float refraction_angle = std::sqrt(1 - std::pow(k, 2) * std::pow(get_angle_sin(angle_of_incidence), 2));
            Vector refract_dir = *ray.direction * k + (k * angle_of_incidence - refraction_angle) * tmp_norm;

            // Offset starting point so it doesn't hit itself 
            Point refract_start = refract_dir*norm < 0 ? intersection_point - norm * 0.001 : 
                                                        intersection_point + norm * 0.001;
            Ray refraction_ray(refract_start, refract_dir.normalize());

            // Casting refraction ray
            refraction_result = cast_ray(refraction_ray, objects, lights, depth + 1) * 0.8;
        } else {
            reflection_result = reflection_result * 0.9;
        }

        RGB glare = get_color(WHITE) * brightness * intersected_obj.get_mirror_coef();
        result = (reflection_result + refraction_result) * 0.9 + glare;

    }

    return result;
}



char * get_png_data(const std::vector<std::vector<RGB> > &pix, const int &w, const int &h) {
    char *result = new char[w * h * 3 + 3];
    
    int i = 0;
    int offset = w * h;
    for (auto row: pix) {
        for (auto rgb: row) {
            result[i] = (char)rgb.get_r();
            result[i + 1] = (char)rgb.get_g();
            result[i + 2] = (char)rgb.get_b();
            i += 3;
        }
    }
    return result;
}


void render (const std::vector<Sphere> &objects, const std::vector<Light> &lights,
             const int &w = width, const int &h = height) {
    auto start_time = std::chrono::steady_clock::now();
    // pix - pixel matrix for picture generation
    std::vector<std::vector<RGB> > pix(h, std::vector<RGB>(w));
    omp_set_num_threads(32);
    #pragma omp target teams distribute parallel for collapse(2)
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            Point start = Point(w/2, h/2, -w/2);    // Point of view
            Vector direction = Vector(start, Point(j, i, 0)).normalize();   //current ray direction
            Ray ray = Ray(start, direction);
            pix[i][j] = cast_ray(ray, objects, lights);
        }
    }
    #pragma omp barrier

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<float> calculation_time = end_time - start_time;
    std::cout << "Render time: " << calculation_time.count() << "s" << std::endl;

    // Generating picture in png format
    char *picture = get_png_data(pix, w, h);
    stbi_write_png("result.png", w, h, 3, picture, w * 3);
    delete [] picture;

}


// Comparator for objects - closest first
bool comparator (const Sphere &s1, const Sphere &s2) {
    return s1.get_position().get_z() <= s2.get_position().get_z();
}


int main (int argc, char **argv) {
    srand(time(NULL));

    std::vector<Sphere> objects;
    std::vector<Light> lights;

    // Adding lights
    lights.push_back(Light(Point( 3 * width/4, 0, -100), 0.5));
    lights.push_back(Light(Point(width/5, 0, 100), 0.5));
    lights.push_back(Light(Point(width/2, height/2, -200), 0.25));

    // Adding objects
    objects.push_back(Sphere(300, Point(300, 540, 900), get_material(PLASTIC, RED)));    // Red
    objects.push_back(Sphere(150, Point(1400, 800, 600), get_material(PLASTIC, PURPLE))); // Purple
    objects.push_back(Sphere(200, Point(700, 600, 400), get_material(GLASS)));  // transparent
    objects.push_back(Sphere(200, Point(width/2, -200, 1100), get_material(METAL))); // mirror
    objects.push_back(Sphere(300, Point(1700, 400, 500), get_material(PLASTIC, BLUE))); // Blue 2

    // Start rendering
    render(objects, lights, 1920, 1080);
    std::cout << "Ready!" << std::endl;

    return 0;
}