//
// Created by Phil Lan on 2023-11-24.
//

#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

struct AABB {
    glm::vec3 size;
    glm::vec3 center;

    AABB(const glm::vec3 &size, const glm::vec3 &center);
};

#endif //AABB_H
