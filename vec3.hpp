#ifndef VEC3_H
#define VEC3_H

#include "common.hpp"
#include <cmath>
#include <iostream>

class Vec3 {
    public:
        double e[3];

        Vec3() : e{0, 0, 0} {
        }
        Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {
        }

        double x() const {
            return e[0];
        }
        double y() const {
            return e[1];
        }
        double z() const {
            return e[2];
        }

        Vec3 operator-() const {
            return Vec3(-e[0], -e[1], -e[2]);
        }

        const double &operator[](int i) const {
            return e[i];
        }

        double &operator[](int i) {
            return e[i];
        }

        Vec3 &operator+=(const Vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        Vec3 &operator*=(double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        Vec3 &operator/=(double t) {
            return *this *= 1 / t;
        }

        double length() const {
            return std::sqrt(length_squared());
        }

        double length_squared() const {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }

        // returns true if the vector is close to zero in all dimensions
        bool near_zero() const {
            auto s = 1e-8;
            return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) &&
                   (std::fabs(e[2]) < s);
        }

        static Vec3 random() {
            return Vec3(random_double(), random_double(), random_double());
        }

        static Vec3 random(double min, double max) {
            return Vec3(random_double(min, max), random_double(min, max),
                        random_double(min, max));
        }
};

using Point3 = Vec3;

// Vector utility functions

inline std::ostream &operator<<(std::ostream &out, const Vec3 &v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
    return Vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
    return Vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
    return Vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline Vec3 operator*(const double t, const Vec3 &u) {
    return Vec3(t * u[0], t * u[1], t * u[2]);
}

inline Vec3 operator*(const Vec3 &u, const double t) {
    return t * u;
}

inline Vec3 operator/(const Vec3 &v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vec3 &u, const Vec3 &v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
    return Vec3(u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2],
                u[0] * v[1] - u[1] * v[0]);
}

inline Vec3 unit_vector(const Vec3 &v) {
    return v / v.length();
}

inline Vec3 random_in_unit_disk() {
    while (true) {
        auto p = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() < 1) return p;
    }
}

inline Vec3 random_unit_vector() {
    while (true) {
        auto p = Vec3::random(-1, 1);
        auto len_sq = p.length_squared();
        // filter out any values that may cause floating point
        // precision errors or are outside the circle
        if (1e-160 < len_sq && len_sq <= 1) {
            return p / sqrt(len_sq);
        }
    }
}

inline Vec3 random_on_hemisphere(const Vec3 &normal) {
    Vec3 on_unit_circle = random_unit_vector();
    // if on the same hemisphere, return self. Otherwise invert.
    if (dot(on_unit_circle, normal) > 0.0) {
        return on_unit_circle;
    }
    return -on_unit_circle;
}

inline Vec3 reflect(const Vec3 &v, const Vec3 &n) {
    return v - (2 * dot(v, n) * n);
}

inline Vec3 refract(const Vec3 &uv, const Vec3 &n, double eta_ratio) {
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    Vec3 r_out_perp = eta_ratio * (uv + cos_theta * n);
    Vec3 r_out_parallel =
        -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif
