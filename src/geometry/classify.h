//
// Created by Phil Lan on 2023-11-26.
//

#ifndef CLASSIFY_H
#define CLASSIFY_H

#include "AABB.h"
#include "Plane.h"

/**
 * \brief Check if an AABB and a plane intersect
 * \param aabb AABB
 * \param plane Plane
 * \return 0 if box intersects plane, signed distance between center of box and plane otherwise
 */
float classify(const AABB &aabb, const Plane &plane);




#endif //CLASSIFY_H
