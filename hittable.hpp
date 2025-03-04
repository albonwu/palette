#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.hpp"

struct hit_record {
    point3 p;
    vec3 normal;
    double t;
};

class hittable {
    public:
        virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& record) const = 0;

        virtual ~hittable() = default;
};

#endif