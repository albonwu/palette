#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.hpp"
#include "vec3.hpp"

bool in_range(double root, double min, double max) {
    return min < root && root < max;
}

class Sphere : public Hittable {
    private:
        point3 center;
        double radius;

    public:
        // Ensures non-negative radius
        Sphere(const vec3 &center, double radius)
            : center(center), radius(std::fmax(0, radius)) {
        }

        bool hit(const ray &r, double ray_tmin, double ray_tmax,
                 hit_record &rec) const override {
            vec3 origin_center = center - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), origin_center);
            auto c = origin_center.length_squared() - (radius * radius);

            auto discriminant = h * h - a * c;
            if (discriminant < 0) {
                return false;
            }

            auto sqrtd = std::sqrt(discriminant);

            auto root = (h - sqrtd) / a;
            if (!in_range(root, ray_tmin, ray_tmax)) {
                root = (h + sqrtd) / a;
                if (!in_range(root, ray_tmin, ray_tmax)) {
                    return false;
                }
            }

            rec.p = r.at(root);
            rec.normal = (rec.p - center) / radius;
            rec.t = root;
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);

            return true;
        }
};

#endif
