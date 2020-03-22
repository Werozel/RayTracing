#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <chrono>
#include "vectors.h"
#include "utils.cpp"
#include "objects.h"

const std::string output_file = "result.ppm";
const RGB backgroundColor = RGB(114, 237, 237);
const int width = 1920;
const int height = 1080;
const float view_angle = 90;


RGB cast_ray(const Ray &ray, const std::vector<Object> &objects) {
    for (Object obj: objects) {
        if (obj.ray_intersection(ray)) return obj.color;
    }
    return backgroundColor;
}


void render (const std::vector<Object> &objects, const int &w = width, const int &h = height) {

    std::ofstream out;
    out.open(output_file);

    std::vector<std::vector<RGB> > pix(h, std::vector<RGB>(w));
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            float fw = (float)w;
            float fh = (float)h;
            float x =  (2*(i + 0.5)/fw  - 1)*tan(view_angle/2.)*w/fh;
            float y = -(2*(j + 0.5)/fh - 1)*tan(view_angle/2.);
            Vector direction = Vector(x, y, -1).normalize();
            Ray ray = Ray(Point(0, 0, 0), direction);
            pix[i][j] = cast_ray(ray, objects);
        }
    }

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

    std::vector<Object> objects;
    objects.push_back(Sphere(20, RGB(64, 247, 39), Point(200, 100, 10), OPAQUE));

    render(objects);

    return 0;
}