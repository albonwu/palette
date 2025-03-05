#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

class ray {
    public:
        ray() {
        }

        ray(const point3 &origin, const Vec3 &direction)
            : orig(origin), dir(direction) {
        }

        const point3 &origin() const {
            return orig;
        }
        const Vec3 &direction() const {
            return dir;
        }

        /**
         * Returns the 3d point the ray points to for a given t
         */
        point3 at(double t) const {
            return orig + (t * dir);
        }

    private:
        point3 orig;
        Vec3 dir;
};

#endif
