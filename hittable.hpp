#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.hpp"

struct hit_record {
        point3 p;
        vec3 normal;
        double t;
        bool front_face;

        void set_face_normal(const ray &r, const vec3 &outward_normal) {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class Hittable {
    public:
        virtual bool hit(const ray &r, double ray_tmin, double ray_tmax,
                         hit_record &record) const = 0;

        virtual ~Hittable() = default;
};

#endif
