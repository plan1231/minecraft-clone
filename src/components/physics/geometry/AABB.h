//
// Created by Phil Lan on 2023-11-24.
//

#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    glm::vec3 halfSize() const {return (max - min) / 2.0f;}
    glm::vec3 center() const {return (max + min) / 2.0f;}
};

#endif //AABB_H
