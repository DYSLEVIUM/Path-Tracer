//Compile and run: g++ -g -Wshadow -Wall main.cpp -o a.exe -Ofast -Wno-unused-result

#include <iostream>

#include "../include/color.h"
#include "../include/image.h"
#include "../include/vec3.h"

int32_t main() {
    //  Image
    Image<int> img(256, 256);

    std::cout << "P3\n"
              << img.getWidth() << ' ' << img.getHeight() << "\n255\n";

    for (int32_t i = 0; i < img.getHeight(); ++i) {
        for (int32_t j = 0; j < img.getWidth(); ++j) {
            color pixel_color((double(i)) / (img.getWidth() - 1), (double(j)) / (img.getHeight() - 1), 0.25);

            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}