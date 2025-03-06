#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.hpp"
#include "material.hpp"
#include "color.hpp"
#include "vec3.hpp"

class Camera {
    public:
        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 10;
        int max_depth = 10;

        void render(const Hittable &world) {
            initialize();

            std::cout << "P3\n"
                      << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height - j)
                          << ' ' << std::flush;
                for (int i = 0; i < image_width; i++) {
                    Color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        Ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    write_color(std::cout, pixel_samples_scale * pixel_color);
                }
            }

            std::clog << "\rDone.                 \n";
        }

    private:
        int image_height;
        double pixel_samples_scale;
        point3 center;
        point3 pixel_00_loc; // Location of pixel 0, 0
        Vec3 pixel_delta_u;  // Horizontal pixel offset vector
        Vec3 pixel_delta_v;  // Vertical pixel offset vector

        void initialize() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = point3(0, 0, 0);

            auto focal_length = 1.0;
            auto viewport_height = 2.0;
            auto viewport_width =
                viewport_height * (double(image_width) / image_height);

            auto viewport_u = Vec3(viewport_width, 0, 0);
            auto viewport_v = Vec3(0, -viewport_height, 0);

            // Scale offset according to viewport
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            auto viewport_upper_left = center - Vec3(0, 0, focal_length) -
                                       viewport_u / 2 - viewport_v / 2;
            pixel_00_loc =
                viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        // Compute ray using location of pixel 0, 0 and antialiasing
        Ray get_ray(int i, int j) const {
            auto offset = sample_square();
            auto pixel_sample = pixel_00_loc +
                                ((i + offset.x()) * pixel_delta_u) +
                                ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = center;
            auto ray_direction = pixel_sample - ray_origin;

            return Ray(ray_origin, ray_direction);
        }

        Vec3 sample_square() const {
            return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        Color ray_color(const Ray &r, int depth, const Hittable &world) const {
            // after maximum ray bounces, stop gathering light information
            if (depth <= 0) {
                return Color(0, 0, 0);
            }

            hit_record rec;

            // ignore floating point error hits
            if (world.hit(r, Interval(0.001, infinity), rec)) {
                Ray scattered;
                Color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                    return attenuation * ray_color(scattered, depth - 1, world);
                }
                return Color(0, 0, 0);
            }

            // If no hits, render sky
            Vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
        }
};

#endif
