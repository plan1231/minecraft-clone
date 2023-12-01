//
// Created by Phil Lan on 2023-11-24.
//

#ifndef PLANE_H
#define PLANE_H

#include <../../../cmake-build-debug/_deps/glm-src/glm/glm.hpp>

struct Plane {
    /**
     * \brief Plane normal. Has to be unit length.
     */
    glm::vec3 normal;
    /**
     * \brief signed distance to parallel plane that passes through the origin
     */
    float distance;
};



#endif //PLANE_H
