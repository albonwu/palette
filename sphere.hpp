#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.hpp"
#include "vec3.hpp"
#include "interval.hpp"

bool in_range(double root, double min, double max) {
    return min < root && root < max;
}

class Sphere : public Hittable {
    private:
        point3 center;
        double radius;

    public:
        // Ensures non-negative radius
        Sphere(const Vec3 &center, double radius)
            : center(center), radius(std::fmax(0, radius)) {
        }

        bool hit(const ray &r, Interval ray_t, hit_record &rec) const override {
            Vec3 origin_center = center - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), origin_center);
            auto c = origin_center.length_squared() - (radius * radius);

            auto discriminant = h * h - a * c;
            if (discriminant < 0) {
                return false;
            }

            auto sqrtd = std::sqrt(discriminant);

            auto root = (h - sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                root = (h + sqrtd) / a;
                if (!ray_t.surrounds(root)) {
                    return false;
                }
            }

            rec.p = r.at(root);
            rec.normal = (rec.p - center) / radius;
            rec.t = root;
            Vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);

            return true;
        }
};

#endif
