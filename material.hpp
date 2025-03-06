#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.hpp"
#include "color.hpp"

class Material {
    public:
        virtual ~Material() = default;

        virtual bool scatter(const Ray &r_in, const hit_record &rec,
                             Color &attenuation, Ray &scattered) const {
            return false;
        }
};

class Lambertian : public Material {
    public:
        Lambertian(const Color &albedo) : albedo(albedo) {
        }

        bool scatter(const Ray &r_in, const hit_record &rec, Color &attenuation,
                     Ray &scattered) const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            // prevent degenerate scatter direction
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered = Ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        Color albedo;
};

class Metal : public Material {
    public:
        Metal(const Color &albedo, double fuzz)
            : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {
        }

        bool scatter(const Ray &r_in, const hit_record &rec, Color &attenuation,
                     Ray &scattered) const override {
            Vec3 reflected = reflect(r_in.direction(), rec.normal);
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
            scattered = Ray(rec.p, reflected);
            attenuation = albedo;
            return dot(scattered.direction(), rec.normal) > 0;
        }

    private:
        Color albedo;
        double fuzz;
};

class Dielectric : public Material {
    public:
        Dielectric(double refraction_index)
            : refraction_index(refraction_index) {
        }

        bool scatter(const Ray &r_in, const hit_record &rec, Color &attenuation,
                     Ray &scattered) const override {
            attenuation = Color(1.0, 1.0, 1.0);
            double ri =
                rec.front_face ? (1.0 / refraction_index) : refraction_index;

            Vec3 unit_direction = unit_vector(r_in.direction());
            Vec3 refracted = refract(unit_direction, rec.normal, ri);

            scattered = Ray(rec.p, refracted);
            return true;
        }

    private:
        // https://en.wikipedia.org/wiki/Refractive_index
        double refraction_index;
};

#endif
