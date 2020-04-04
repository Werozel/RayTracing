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
    if (objects.size() == 0) return get_material_color();
    if (depth == recursion_gap) return get_material_color();

    Point intersection_point = no_intersection;
    float min_dist = std::numeric_limits<float>::max();
    Object *intersected_obj = objects[0];
    int obj_i = 0;
    int i = 0;
    for (const auto& obj: objects) {
        // Returns Point(-1, -1, -1) if no intersection detected
        Point curr_intersection = obj->ray_intersection(ray); // Intersection point of ray
        if (curr_intersection != no_intersection) {
            float curr_distance = distance(curr_intersection, ray.get_start());
            if (curr_distance < min_dist) {
                min_dist = curr_distance;
                intersection_point = curr_intersection;
                intersected_obj = obj;
                obj_i = i;
            }
        }
        i++;
    }

    if (intersection_point == no_intersection)  { return get_material_color();}

    Vector norm = intersected_obj->get_norm(intersection_point);
    
    // Calculating brightness
    float brightness = 0; // Brightnesss of intersection point
    for (const auto& light: lights) {
        Vector vector_of_incidence = Vector(intersection_point, light.get_position()).normalize(); // To light direction

        Ray to_light(intersection_point, vector_of_incidence);
        bool shade_flag = false; // Flag indicating if point is in shade for current light
        for (int j = 0; j < objects.size(); j++) {
            if (j == obj_i) continue; // Skipping current object
            Point shade_hit_point = objects[j]->ray_intersection(to_light);
            if (shade_hit_point != no_intersection) {
                if (distance(shade_hit_point, intersection_point) <= 
                    distance(intersection_point, light.get_position())) {
                    shade_flag = true;
                    break;
                }
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
            brightness += light.intensity * 0.33 * intersected_obj->get_mirror_coef() * std::pow(angle_of_reflection, intersected_obj->get_shininess());
        }
    }
    
    // Getting the color of the point
    RGB result = get_material_color();
    if (intersected_obj->get_stype() == OPAQUE) {
        
        return (brightness == 0.0) ? get_material_color(BLACK) : intersected_obj->get_color(intersection_point) * brightness;

    } else {       // if MIRROR or TRANSPARENT
        Vector reflect_dir = ray.get_direction() - 2 * (ray.get_direction() * norm) * norm;
        Point reflect_start = reflect_dir*norm < 0 ? intersection_point - norm * 0.001 : 
                                                    intersection_point + norm * 0.001;
        Ray reflected_ray(reflect_start, reflect_dir.normalize());
        RGB reflection_result = cast_ray(reflected_ray, objects, lights, depth + 1);

        RGB refraction_result(0, 0, 0);

        if (intersected_obj->get_stype() == TRANSPARENT) {
            reflection_result = reflection_result * 0.05;        

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
            refraction_result = cast_ray(refraction_ray, objects, lights, depth + 1) * 0.9;
        } else {
            reflection_result = reflection_result * 0.9;
        }

        RGB glare = (brightness == 0.0) ? get_material_color(BLACK) : 
                        get_material_color(WHITE) * brightness * intersected_obj->get_mirror_coef();
        result = (reflection_result + refraction_result) * 0.9 + glare;

    }

    return result;
}



char * get_png_data(RGB **pix, const int &w, const int &h) {
    char *result = new char[w * h * 3 + 3];
    
    int k = 0;
    int offset = w * h;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            pix[i][j].normalize();
            result[k] = (char)pix[i][j].get_r();
            result[k + 1] = (char)pix[i][j].get_g();
            result[k + 2] = (char)pix[i][j].get_b();
            k += 3;
        }
    }
    return result;
}


void render (const std::vector<Object *> &objects,
             const std::vector<Light> &lights,
             const int &w = width, const int &h = height, 
             const int &smoothness = 1) {
    auto start_time = std::chrono::steady_clock::now();
    // pix - pixel matrix for picture generation
    RGB **pix = new RGB *[h];
    for (int i = 0; i < h; i++) {
        pix[i] = new RGB[w];
    }

    Point start(w/2, h/2, -w/2);
    float k = 1.f / (float)smoothness;

    omp_set_num_threads(threads_num);
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            Vector direction;
            Ray ray;
            RGB res;   
            for (int t = 0; t < smoothness; t++) {
                direction = Vector(start, Point(j + k*t, i + k*t, 0)).normalize();
                ray = Ray(start, direction);
                res += cast_ray(ray, objects, lights);
            }
            pix[i][j] = res / (float)smoothness;
        }
    }
    #pragma omp barrier

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<float> calculation_time = end_time - start_time;
    std::cout << "Render time: " << calculation_time.count() << "s" << std::endl;

    // Generating picture in png format
    char *picture = get_png_data(pix, w, h);
    stbi_write_png(output_file, w, h, 3, picture, w * 3);
    delete [] picture;

    for (int i = 0; i < h; i++) {
        delete [] pix[i];
    }
    delete [] pix;

}

// Loads .obj file
void load_object(const std::string &file_name, const Point &pos, 
                 const Material &m, const int &scale, std::vector<Object *> &arr,
                 const int &x_m = 1, const int &y_m = 1, const int &z_m = 1) {
    std::ifstream ifs("obj/" + file_name);
    std::vector<Point> points;
    points.push_back(Point(0, 0, 0));
    char mode;
    float x, y, z;
    while (ifs >> mode) {
        if (mode == 'f') break;
        if (mode != 'v') continue;
        ifs >> x >> y >> z;
        points.push_back(scale * Point(x_m * x, y_m * y, z_m * z) + pos);
    }

    int indexes[4];
    while (!ifs.eof() && mode == 'f') {
        ifs >> indexes[0] >> indexes[1] >> indexes[2];
        Point p1 = points[indexes[0]], p2 = points[indexes[1]], p3 = points[indexes[2]];

        if (!ifs.eof() && ifs.peek() != '\n') {
            ifs >> indexes[3];
            Point p4 = points[indexes[3]];
            Point poly_center = (p1 + p2 + p3 + p4) / 4;
            arr.push_back(new Rectangle(poly_center, m, p1, p2, p3, p4));
        } else {
            Point poly_center = (p1 + p2 + p3) / 3;
            arr.push_back(new Polygon(poly_center, m, p1, p2, p3));
        }
        while(ifs >> mode && mode != 'f');
    }
    ifs.close();
}

int main (int argc, char **argv) {
    srand(time(NULL));
    int smoothness = 1;

    strcpy(output_file, "result.png");
    for (int i = 1; i < argc - 1; i+=2) {
        if (strcmp(argv[i], "-out") == 0) {
            strcpy(output_file, argv[i + 1]);
        } else if (strcmp(argv[i], "-scene") == 0) {
            scene_number = atoi(argv[i + 1]);
            if (scene_number < 1 || 3 < scene_number) return 0;
        } else if (strcmp(argv[i], "-threads") == 0) {
            threads_num = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-smooth") == 0) {
            smoothness = atoi(argv[i + 1]);
        } else {
            continue;
        }
    }
    if (scene_number == 0) return 0;

    std::vector<Object *> objects;
    std::vector<Polygon> polygons;
    std::vector<Light> lights;

    // Adding lights
    lights.push_back(Light(Point( 1500, -350, -300), 0.5));
    lights.push_back(Light(Point(430, 0, -100), 0.75));
    // lights.push_back(Light(Point(1000, 550, -400), 0.4));

    // Adding objects
    objects.push_back(new Sphere(300, Point(150, 540, 700), get_material(PLASTIC, BLUE)));    // Blue under the tree
    objects.push_back(new Sphere(150, Point(1250, 800, 400), get_material(METAL, BLUE))); // Mirror on the right
    objects.push_back(new Sphere(200, Point(550, 700, 200), get_material(GLASS)));  // Glass under the tree
    // objects.push_back(new Sphere(200, Point(width/2, -200, 1100), get_material(METAL))); // mirror
    objects.push_back(new Sphere(300, Point(1550, 100, 300), get_material(PLASTIC, RED))); // Red in the air

    objects.push_back(new SceneFloor(Point(950, 950, 0), get_material(PLASTIC, DARK_PINK), LIGHT_BLUE, 200));   // Floor

    // Loading objects
    // load_object("duck.obj", Point(1250, 750, 450), get_material(PLASTIC, ORANGE), 60, objects, 1, -1, 1);
    // load_object("Palm_Tree_leaves.obj", Point(250, 950, 150), get_material(PLASTIC, GREEN), 150, objects, 1, -1, 1);
    // load_object("Palm_Tree_trunk.obj", Point(250, 950, 150), get_material(PLASTIC, BROWN), 150, objects, 1, -1, 1);
    // load_object("bust.obj", Point(width/2, height - 100, 200), get_material(PLASTIC, WHITE), 250, objects, -1, -1, -1);
    load_object("cube.obj", Point(1400, 850, 0), get_material(PLASTIC, GREEN), 100, objects);
    load_object("Octahedron.obj", Point(200, 700, 0), get_material(PLASTIC, YELLOW), 200, objects);
    // load_object("David.obj", Point(width/2, height - 100, 100), get_material(PLASTIC, WHITE), 2, objects);
    // load_object("Discobolus.obj", Point(width/2 + 50, height - 100, 0), get_material(PLASTIC, WHITE), 10, objects, 1, -1, 1);

    printf("Loaded %d objects\n", (int)objects.size());

    // Start rendering
    std::cout << "Started" << std::endl;
    render(objects, lights, 1920, 1080, smoothness);
    std::cout << "Ready!" << std::endl;

    for (int i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
    delete [] output_file;

    return 0;
}