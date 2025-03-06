#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "common.hpp"
#include "hittable.hpp"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable {
    public:
        // shared_ptr prevents object slicing
        std::vector<shared_ptr<Hittable>> objects;

        HittableList() {
        }
        HittableList(shared_ptr<Hittable> object) {
            add(object);
        }

        void clear() {
            objects.clear();
        }

        void add(shared_ptr<Hittable> object) {
            objects.push_back(object);
        }

        bool hit(const Ray &r, Interval ray_t, hit_record &rec) const override {
            hit_record temp_rec;
            bool any_hits = false;
            auto closest_so_far = ray_t.max;

            for (const auto &object : objects) {
                if (object->hit(r, Interval(ray_t.min, closest_so_far),
                                temp_rec)) {
                    any_hits = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return any_hits;
        }
};

#endif
