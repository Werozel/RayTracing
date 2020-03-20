#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include "vectors.h"
#include "utils.cpp"

const std::string output_file = "result.ppm";
const int width = 1920;
const int height = 1080;

void render (const std::vector<std::vector<RGB> > &arr, const int &w = width, const int &h = height) {

    std::ofstream out;
    out.open(output_file);

    out << "P6\n" << w << " " << h << "\n255\n";
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            out << arr[i][j] << std::endl;
        }
    }

    out.close();
}


int main (int argc, char **argv) {
    
    srand(time(NULL));

    std::vector<std::vector<RGB> > arr(height, std::vector<RGB>(width));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int r = randrange(0, 255);
            int g = randrange(0, 255);
            int b = randrange(0, 255);
            arr[i][j] = RGB(r, g, b);
        }
    }

    render(arr);

    return 0;
}