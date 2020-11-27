//Compile: g++ -g -Wshadow -Wall main.cpp -o a.exe -Ofast -Wno-unused-result && a.exe
#include <cstdio>
#include <iostream>

#include "../include/camera.h"
#include "../include/color.h"
#include "../include/hittable_list.h"
#include "../include/image.h"
#include "../include/material.h"
#include "../include/ray.h"
#include "../include/rtweekend.h"
#include "../include/sphere.h"
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

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    if (depth <= 0) return color(0, 0, 0);

    if (world.hit(r, 0, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());  //  -1.0<=y<=1
    auto t = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);  //  linear interpolation
}

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.45);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int32_t main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    freopen("img.ppm", "w", stdout);
    //  image

    double aspect_ratio = 16.0 / 9.0;
    int imgWidth = 1920;
    Image<int> img(imgWidth, static_cast<int>(imgWidth / aspect_ratio));
    const int sample_per_pixel = 500;
    const int max_depth = 50;

    //  world
    auto world = random_scene();

    //  camera
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    //  render

    std::cout << "P3\n"
              << img.getWidth() << ' ' << img.getHeight() << "\n255\n";

    for (int j = img.getHeight() - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < img.getWidth(); ++i) {
            color pixel_color;
            for (int s = 0; s < sample_per_pixel; ++s) {
                auto u = (i + random_double()) / (img.getWidth() - 1);
                auto v = (j + random_double()) / (img.getHeight() - 1);

                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            write_color(std::cout, pixel_color, sample_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
    // system("start chrome https://www.kylepaulsen.com/stuff/NetpbmViewer/");  //  website to view image
}