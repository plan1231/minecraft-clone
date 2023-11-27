//
// Created by Phil Lan on 2023-11-24.
//

#include "Frustrum.h"

#include "classify.h"
bool Frustrum::contains(const AABB& aabb) {
    for(const auto &plane : this->planes) {
        if(classify(aabb, plane) < 0.0f) return false;
    }
    return true;
}
