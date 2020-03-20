#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include "vectors.h"
#include "utils.cpp"

const std::string output_file = "result.ppm";
const int width = 1080;
const int height = 1920;

void render(const std::vector<std::vector<RGB> > &arr, const int &w = width, const int &h = height) {

    std::ofstream out;
    out.open(output_file);

    out << "P6\n" << w << " " << h << "\n255\n";
    std::cout << "file created" << std::endl;
    printf("w = %d, h = %d\n", w, h);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            out << arr[i][j] << std::endl;
        }
    }

    out.close();
}


int main (int argc, char **argv) {
    
    srand(time(NULL));

    std::vector<std::vector<RGB> > arr(2, std::vector<RGB>(3));

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            int r = randrange(0, 255);
            int g = randrange(0, 255);
            int b = randrange(0, 255);
            arr[i][j] = RGB(r, g, b);
        }
    }

    std::cout << "rendering" << std::endl;
    render(arr, 3, 2);

    return 0;
}