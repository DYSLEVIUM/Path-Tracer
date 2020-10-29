//Compile: g++ -g -Wshadow -Wall main.cpp -o a.exe -Ofast -Wno-unused-result

#include <iostream>

#include "../include/color.h"
#include "../include/image.h"
#include "../include/ray.h"
#include "../include/vec3.h"

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();         //  a = b*b
    auto half_b = dot(oc, r.direction());            //  b = 2*b*(a-c)
    auto c = oc.length_squared() - radius * radius;  //  c = (a-c)*(a-c) - r^2
    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0) return -1.0;

    return (-half_b - sqrt(discriminant)) / a;
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    vec3 unit_direction = unit_vector(r.direction());                    //  -1.0<=y<=1
    t = 0.5 * (unit_direction.y() + 1.0);                                //  0.0<=y<=1
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);  //  linear interpolation
}

int32_t main() {
    //  image

    double aspect_ratio = 16.0 / 9.0;
    int imgWidth = 300;
    Image<int> img(imgWidth, static_cast<int>(imgWidth / aspect_ratio));

    //  camera

    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;

    point3 origin(0, 0, 0);
    vec3 horizontal(viewport_width, 0, 0);
    vec3 vertical(0, viewport_height, 0);
    vec3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

    //  render

    std::cout << "P3\n"
              << img.getWidth() << ' ' << img.getHeight() << "\n255\n";

    for (int j = img.getHeight() - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < img.getWidth(); ++i) {
            double u = double(i) / (img.getWidth() - 1);
            double v = double(j) / (img.getHeight() - 1);

            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);

            color pixel_color = ray_color(r);

            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}