//
// Created by Phil Lan on 2023-11-24.
//

#ifndef FRUSTRUM_H
#define FRUSTRUM_H

#include "Plane.h"
#include "AABB.h"

struct Frustrum {
    union {
        struct {
            Plane top;
            Plane bottom;
            Plane left;
            Plane right;
            Plane near;
            Plane far;
        };

        Plane planes[6];
    };
    bool contains(const AABB &aabb);
};



#endif //FRUSTRUM_H
