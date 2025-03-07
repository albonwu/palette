#ifndef HITTABLE_H
#define HITTABLE_H

// #include "common.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include "interval.hpp"

class Material;

struct hit_record {
        Point3 p;
        Vec3 normal;
        shared_ptr<Material> mat;
        double t;
        bool front_face;

        void set_face_normal(const Ray &r, const Vec3 &outward_normal) {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class Hittable {
    public:
        virtual bool hit(const Ray &r, Interval ray_t,
                         hit_record &record) const = 0;

        virtual ~Hittable() = default;
};

#endif
