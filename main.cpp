#include <iostream>
#include "vec3.hpp"
#include "color.hpp"
#include "ray.hpp"

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 origin_center = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), origin_center);
    auto c = origin_center.length_squared() - (radius * radius);
    
    auto discriminant = h * h - a * c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (h - std::sqrt(discriminant)) / a;
    }
}

// return color for a given scene ray
color ray_color(const ray& r) {
    point3 sphere_center(0, 0, -1);
    auto hit_point = hit_sphere(sphere_center, 0.5, r);
    if (hit_point > 0){
        vec3 n = unit_vector(r.at(hit_point) - sphere_center);
        return 0.5 * color(n.x() + 1, n.y() + 1, n.z() + 1);
    }
    vec3 unit_dir = unit_vector(r.direction());
    auto a = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // create a simple camera
    double focal_length = 1.0;              // distance between camera and viewport
    double total_viewpoint_height = 2.0;
    double total_viewpoint_width = total_viewpoint_height * (double(image_width) / image_height);
    point3 camera_center = point3(0, 0, 0);

    // calculate vectors spanning viewpoint edges
    vec3 viewpoint_w = vec3(total_viewpoint_width, 0, 0);
    vec3 viewpoint_h = vec3(0, -total_viewpoint_height, 0);

    // calculate pixel width/height
    vec3 pixel_delta_w = viewpoint_w / image_width;
    vec3 pixel_delta_h = viewpoint_h / image_height;

    // calculate the top left pixel location
    point3 viewpoint_top_left = camera_center - vec3(0, 0, focal_length) - (viewpoint_h / 2) - (viewpoint_w / 2);
    point3 pixel_00_loc = viewpoint_top_left + 0.5 * (pixel_delta_w + pixel_delta_h);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' '
                  << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel_00_loc + (i * pixel_delta_w) + (j * pixel_delta_h);
            auto ray_dir = pixel_center - camera_center; 
            ray r(camera_center, ray_dir);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.\n";

}
