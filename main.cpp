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
    Point no_intersection(-1, -1, -1);
    for (int i = 0; i < objects.size(); i++) {
        // Returns Point(-1, -1, -1) if no intersection detected
        Point intersection_point = objects[i].ray_intersection(ray); // Intersection point of ray
        if (intersection_point != no_intersection) {
            
            Vector norm = Vector(objects[i].get_position(), intersection_point).normalize();
            float brightness = 0; // Brightnesss of intersection point
            for (auto light: lights) {
                Vector vector_of_incidence(intersection_point, light.position); // To light direction
                
                Ray to_light(intersection_point, vector_of_incidence.normalize());
                bool shade_flag = false; // Flag indicating if point is in shade for current light
                for (int j = 0; j < objects.size(); j++) {
                    if (j == i) continue; // Skipping current object
                    if (objects[j].ray_intersection(to_light) != no_intersection) {
                        shade_flag = true;
                        break;
                    }
                }
                if (shade_flag) continue; // If in shade skipping brightness calculation

                float angle_of_incidence = get_angle(norm, vector_of_incidence);
                if (angle_of_incidence > 0) {
                    brightness += angle_of_incidence * light.intensity;

                    // Calculating glares
                    // Direction to camera from intersection point
                    Vector to_camera = 2 * (vector_of_incidence * norm) * norm - vector_of_incidence;
                    float angle_of_reflection = get_angle(norm, to_camera);
                    // Calculating a glare ---  K * (n * to_camera)^p
                    brightness += light.intensity * std::pow(angle_of_reflection, objects[i].get_shininess());
                }
                
            }
            return *objects[i].color * brightness;
        }
    }
    return backgroundColor;
}


void render (const std::vector<Sphere> &objects, const std::vector<Light> &lights,
             const int &w = width, const int &h = height) {

    // pix - pixel matrix for picture generation
    std::vector<std::vector<RGB> > pix(h, std::vector<RGB>(w));
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            Point start = Point(w/2, h/2, -w/2);    // Point of view
            Vector direction = Vector(start, Point(j, i, 0)).normalize();   //current ray direction
            Ray ray = Ray(start, direction);
            pix[i][j] = cast_ray(ray, objects, lights);
        }
    }


    // Generating picture in ppm P6 format
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


// Comparator for objects - closest first
bool comparator (const Sphere &s1, const Sphere &s2) {
    return s1.get_position().get_z() <= s2.get_position().get_z();
}


int main (int argc, char **argv) {
    srand(time(NULL));

    std::vector<Sphere> objects;
    std::vector<Light> lights;

    // Adding lights
    lights.push_back(Light(Point( 3 * width/4, height/6, -100), 0.5));
    lights.push_back(Light(Point(width/5, 0, 100), 0.5));
    lights.push_back(Light(Point(width/2, height/2, -200), 0.25));
    RGB white = RGB(255, 255, 255);

    // Adding objects
    objects.push_back(Sphere(300, RGB(255, 0, 0), Point(300, 540, 800), OPAQUE));    // Red
    objects.push_back(Sphere(150, RGB(162, 1, 202), Point(1400, 800, 600), OPAQUE)); // Purple
    objects.push_back(Sphere(200, RGB(0, 255, 0), Point(500, 540, 500), OPAQUE));  // Green

    // for (auto light: lights) {
    //     objects.push_back(Sphere(30, white, light.get_position(), OPAQUE));
    // }

    // Closest objects first
    std::sort(objects.begin(), objects.end(), comparator);

    auto start_time = std::chrono::steady_clock::now();
    // Start rendering
    render(objects, lights, 1920, 1080);

    // Timer
    std::cout << "Ready!" << std::endl;
    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<float> calculation_time = end_time - start_time;
    std::cout << "Render time: " << calculation_time.count() << "s" << std::endl;

    return 0;
}