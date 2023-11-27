//
// Created by Phil Lan on 2023-11-26.
//
#include "classify.h"
float classify(const AABB& aabb, const Plane& plane) {
    float r = fabs(aabb.size.x * plane.normal.x) + fabs(aabb.size.y * plane.normal.y) + fabs(aabb.size.z * plane.normal.z);
    float d = glm::dot(plane.normal, aabb.center) + plane.distance;
    if (fabs(d) < r) {
        return 0.0;
    }
    else if (d < 0.0) {
        return d + r;
    }
    return d - r;
}