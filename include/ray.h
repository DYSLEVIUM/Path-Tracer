#ifndef RAY_h
#define RAY_h

#include "vec3.h"

class ray {
   public:
    ray() {}
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    point3 origin() const { return this->orig; }
    vec3 direction() const { return this->dir; }

    point3 at(double t) const {
        return orig + t * dir;
    }

   private:
    point3 orig;
    vec3 dir;
};

#endif