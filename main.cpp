#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <chrono>
#include <algorithm>
#include <cstring>
#include <omp.h>
#include "src/constants.h"
#include "src/vectors.h"
#include "src/objects.h"
#include "src/materials.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

char *output_file = new char[100];
int threads_num = 32;
int scene_number = 1;
const Point no_intersection(-1, -1, -1);


template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

RGB cast_ray(const Ray &ray, const std::vector<Object *> &objects,
             const std::vector<Light> &lights, const int &depth = 0) {
    if (objects.size() == 0) return get_color();
    if (depth == recursion_gap) return get_color();

    Point intersection_point = no_intersection;
    float min_dist = std::numeric_limits<float>::max();
    Object *intersected_obj = objects[0];
    int obj_i = 0;
    for (int i = 0; i < objects.size(); i++) {
        // Returns Point(-1, -1, -1) if no intersection detected
        Point curr_intersection = objects[i]->ray_intersection(ray); // Intersection point of ray
        if (curr_intersection != no_intersection) {
            float curr_distance = distance(curr_intersection, ray.get_start());
            if (curr_distance < min_dist) {
                min_dist = curr_distance;
                intersection_point = curr_intersection;
                intersected_obj = objects[i];
                obj_i = i;
            }
        }
    }

    if (intersection_point == no_intersection)  { return get_color();}

    Vector norm = intersected_obj->get_norm(intersection_point);
    
    // Calculating brightness
    float brightness = 0; // Brightnesss of intersection point
    for (auto light: lights) {
        Vector vector_of_incidence = Vector(intersection_point, light.get_position()).normalize(); // To light direction

        Ray to_light(intersection_point, vector_of_incidence);
        bool shade_flag = false; // Flag indicating if point is in shade for current light
        for (int j = 0; j < objects.size(); j++) {
            if (j == obj_i) continue; // Skipping current object
            if (objects[j]->ray_intersection(to_light) != no_intersection) {
                shade_flag = true;
                break;
            }
        }
        if (shade_flag) continue; // If in shade skipping brightness calculation

        float angle_of_incidence = get_angle(norm, vector_of_incidence);
        if (angle_of_incidence > 0) {
            // Calculating deffuse brightness
            if (intersected_obj->get_stype() == OPAQUE ) brightness += angle_of_incidence * light.intensity * intersected_obj->get_deffuse_coef();
            
            // Calculating glare brightness
            // Direction to camera from intersection point
            Vector to_camera = 2 * (vector_of_incidence * norm) * norm - vector_of_incidence;
            float angle_of_reflection = get_angle(norm, to_camera);
            // Calculating a glare ---  K * (n * to_camera)^p
            brightness += light.intensity * intersected_obj->get_mirror_coef() * std::pow(angle_of_reflection, intersected_obj->get_shininess());
        }
    }
    
    // Getting the color of the point
    RGB result = get_color();
    if (intersected_obj->get_stype() == OPAQUE) {
        
        return (brightness == 0.0) ? get_color(BLACK) : intersected_obj->get_color() * brightness;

    } else {       // if MIRROR or TRANSPARENT
        Vector reflect_dir = *ray.direction - 2 * (*ray.direction * norm) * norm;
        Ray reflected_ray(intersection_point, reflect_dir.normalize());
        RGB reflection_result = cast_ray(reflected_ray, objects, lights, depth + 1);

        RGB refraction_result(0, 0, 0);

        if (intersected_obj->get_stype() == TRANSPARENT) {
            reflection_result = reflection_result * 0.2;        

            Vector tmp_norm = norm;  // For calculating an angle between norm and ray direction
            float angle_of_incidence = get_angle(-tmp_norm, ray.get_direction()); // Angle of incidence of current ray
            float n1 = 1; 
            float n2 = intersected_obj->get_refractive_index();
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

        RGB glare = (brightness == 0.0) ? get_color(BLACK) : 
                        get_color(WHITE) * brightness * intersected_obj->get_mirror_coef();
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


void render (const std::vector<Object *> &objects,
             const std::vector<Light> &lights,
             const int &w = width, const int &h = height) {
    auto start_time = std::chrono::steady_clock::now();
    // pix - pixel matrix for picture generation
    std::vector<std::vector<RGB> > pix(h);
    omp_set_num_threads(threads_num);
    for (int i = 0; i < h; i++) {
        std::vector<RGB> row(w);
        #pragma omp parallel for
        for (int j = 0; j < w; j++) {
            Point start = Point(w/2, h/2, -w/2);    // Point of view
            Vector direction = Vector(start, Point(j, i, 0)).normalize();   //current ray direction
            Ray ray = Ray(start, direction);
            row[j] = cast_ray(ray, objects, lights);
        }
        #pragma omp barrier
        pix.push_back(row);
    }

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<float> calculation_time = end_time - start_time;
    std::cout << "Render time: " << calculation_time.count() << "s" << std::endl;

    // Generating picture in png format
    char *picture = get_png_data(pix, w, h);
    stbi_write_png(output_file, w, h, 3, picture, w * 3);
    delete [] picture;

}

// Loads .obj file
void load_object(const std::string &file_name, const Point &pos, const Material &m, const int &scale, std::vector<Object *> *arr) {
    std::ifstream ifs("obj/" + file_name);
    std::vector<Point> points;
    points.push_back(Point(0, 0, 0));
    char mode;
    float x, y, z;
    while (ifs >> mode && mode == 'v') {
        ifs >> x >> y >> z;
        points.push_back(scale * Point(x, -y, z) + pos);
    }

    int i1, i2, i3;
    while (!ifs.eof() && mode == 'f') {
        ifs >> i1 >> i2 >> i3;
        Point p1 = points[i1], p2 = points[i2], p3 = points[i3];

        Point poly_center = (p1 + p2 + p3) / 3;
        Vector to_center = Vector(pos, poly_center);
        arr->push_back(new Polygon(poly_center, m, p2, p1, p3));
        ifs >> mode;
    }
}


int main (int argc, char **argv) {
    srand(time(NULL));

    strcpy(output_file, "result.png");
    for (int i = 1; i < argc - 1; i+=2) {
        if (strcmp(argv[i], "-out") == 0) {
            strcpy(output_file, argv[i + 1]);
        } else if (strcmp(argv[i], "-scene") == 0) {
            scene_number = atoi(argv[i + 1]);
            if (scene_number < 1 || 3 < scene_number) return 0;
        } else if (strcmp(argv[i], "-threads") == 0) {
            threads_num = atoi(argv[i + 1]);
        } else {
            continue;
        }
    }

    std::vector<Object *> objects;
    std::vector<Polygon> polygons;
    std::vector<Light> lights;

    // Adding lights
    lights.push_back(Light(Point( 3 * width/4, 0, -100), 0.5));
    lights.push_back(Light(Point(width/5, 0, 100), 0.5));
    lights.push_back(Light(Point(width/2, height/2, -200), 0.25));

    // Adding objects
    objects.push_back(new Sphere(300, Point(300, 540, 900), get_material(PLASTIC, RED)));    // Red
    objects.push_back(new Sphere(150, Point(1400, 800, 600), get_material(PLASTIC, BLUE))); // Purple
    objects.push_back(new Sphere(200, Point(700, 600, 400), get_material(GLASS)));  // transparent
    // objects.push_back(new Sphere(200, Point(width/2, -200, 1100), get_material(METAL))); // mirror
    objects.push_back(new Sphere(300, Point(1700, 400, 500), get_material(METAL, BLUE))); // Blue 2

    // Loading objects
    load_object("duck.obj", Point(width * 0.75, 2 * height * 0.2, width * 0.3), get_material(PLASTIC, PURPLE), 60, &objects);

    // Start rendering
    std::cout << "Started" << std::endl;
    render(objects, lights, 1920, 1080);
    std::cout << "Ready!" << std::endl;

    for (int i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
    delete [] output_file;

    return 0;
}