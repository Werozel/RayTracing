#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <chrono>
#include "vectors.h"
#include "objects.h"


const std::string output_file = "result.ppm";
const RGB backgroundColor = RGB(114, 237, 237);
const int width = 1920;
const int height = 1080;
const float fov = 90;


RGB cast_ray(const Ray &ray, const std::vector<Sphere> &objects) {
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i].ray_intersection(ray)) {
            return *objects[i].color;
        }
    }
    return backgroundColor;
}


void render (const std::vector<Sphere> &objects, const int &w = width, const int &h = height) {

    std::vector<std::vector<RGB> > pix(h, std::vector<RGB>(w));
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (j == 960 && i == 540) {
                int k = 0;
            }
            Point start = Point(w/2, h/2, -w/2);
            Vector direction = Vector(start, Point(j, i, 0)).normalize();
            Ray ray = Ray(start, direction);
            pix[i][j] = cast_ray(ray, objects);
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


int main (int argc, char **argv) {
    srand(time(NULL));

    std::vector<Sphere> objects;
    objects.push_back(Sphere(300, RGB(255, 0, 0), Point(300, 540, 600), OPAQUE));

    render(objects, 1920, 1080);
    std::cout << "Ready!" << std::endl;

    return 0;
}