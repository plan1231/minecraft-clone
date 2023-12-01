//
// Created by Phil Lan on 2023-11-26.
//

#ifndef CLASSIFY_H
#define CLASSIFY_H

#include <tuple>
#include <tuple>
#include <utility>
#include <utility>
#include <utility>
#include <utility>

#include "components/physics/geometry/AABB.h"
#include "components/physics/geometry/AABB.h"
#include "components/physics/geometry/AABB.h"
#include "components/physics/geometry/AABB.h"
#include "components/physics/geometry/AABB.h"
#include "components/physics/geometry/Plane.h"

/**
 * \brief Check if an AABB and a plane intersect
 * \param aabb AABB
 * \param plane Plane
 * \return 0 if box intersects plane, signed distance between center of box and plane otherwise
 */
float classify(const AABB &aabb, const Plane &plane);

std::tuple<bool, float, glm::vec3> collide(const AABB&s, const AABB&m, const glm::vec3&vel);




#endif //CLASSIFY_H
