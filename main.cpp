#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <chrono>
#include <algorithm>
#include "vectors.h"
#include "objects.h"


const std::string output_file = "result.ppm";
const RGB backgroundColor = RGB(114, 237, 237);
const int width = 1920;
const int height = 1080;
const float fov = 90;


RGB cast_ray(const Ray &ray, const std::vector<Sphere> &objects, const std::vector<Light> &lights) {
    for (int i = 0; i < objects.size(); i++) {
        Point intersection_point = objects[i].ray_intersection(ray);
        if (intersection_point != Point(-1, -1, -1)) {

            Vector norm = Vector(objects[i].get_position(), intersection_point).normalize();
            float brightness = 0;
            for (auto light: lights) {
                Vector vector_of_incidence(intersection_point, light.position);

                Ray to_light(intersection_point, vector_of_incidence.normalize());
                bool shade_flag = false;
                for (int j = 0; j < objects.size(); j++) {
                    if (j == i) continue;
                    if (objects[j].radius != 30 && objects[j].ray_intersection(to_light) != Point(-1, -1, -1)) {
                        shade_flag = true;
                        break;
                    }
                }
                if (shade_flag) continue;

                float angle_of_incidence = get_angle(norm, vector_of_incidence);
                if (angle_of_incidence > 0) {
                    brightness += angle_of_incidence * light.intensity;

                    // No point calculating glares in points hidden from light
                    Vector to_camera = 2 * (vector_of_incidence * norm) * norm - vector_of_incidence;
                    float angle_of_reflection = get_angle(norm, to_camera);
                    brightness += 0.7 * std::pow(angle_of_reflection, objects[i].get_shininess()) * light.intensity;
                }
                
            }
            return *objects[i].color * brightness;
        }
    }
    return backgroundColor;
}


void render (const std::vector<Sphere> &objects, const std::vector<Light> &lights,
             const int &w = width, const int &h = height) {

    std::vector<std::vector<RGB> > pix(h, std::vector<RGB>(w));
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (j == 960 && i == 540) {
                int k = 0;
            }
            Point start = Point(w/2, h/2, -w/2);
            Vector direction = Vector(start, Point(j, i, 0)).normalize();
            Ray ray = Ray(start, direction);
            pix[i][j] = cast_ray(ray, objects, lights);
        }
    }


    std::ofstream out;
    out.open(output_file);

    out << "P6\n" << w << " " << h << "\n255\n";
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            out << pix[i][j];
        }
    }   

    out.close();
}


bool comparator (const Sphere &s1, const Sphere &s2) {
    return s1.get_position().get_z() <= s2.get_position().get_z();
}


int main (int argc, char **argv) {
    srand(time(NULL));

    std::vector<Sphere> objects;
    std::vector<Light> lights;

    lights.push_back(Light(Point( 3 * width/4, height/6, -100), 0.6));
    lights.push_back(Light(Point(width/5, 0, 100), 0.6));
    lights.push_back(Light(Point(width/2, height/2, -200), 0.3));
    RGB white = RGB(255, 255, 255);

    objects.push_back(Sphere(300, RGB(255, 0, 0), Point(300, 540, 800), OPAQUE));    // Red
    objects.push_back(Sphere(150, RGB(162, 1, 202), Point(1400, 800, 600), OPAQUE)); // Purple
    objects.push_back(Sphere(200, RGB(0, 255, 0), Point(500, 540, 500), OPAQUE));  // Green

    // for (auto light: lights) {
    //     objects.push_back(Sphere(30, white, light.get_position(), OPAQUE));
    // }
    // Closest objects first
    std::sort(objects.begin(), objects.end(), comparator);

    auto start_time = std::chrono::steady_clock::now();
    render(objects, lights, 1920, 1080);

    std::cout << "Ready!" << std::endl;
    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<float> calculation_time = end_time - start_time;
    std::cout << "Render time: " << calculation_time.count() << "s" << std::endl;

    return 0;
}