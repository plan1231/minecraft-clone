//
// Created by Phil Lan on 2023-11-26.
//
#include "collide.h"
#include <algorithm>

#include "components/physics/geometry/AABB.h"

float classify(const AABB& aabb, const Plane& plane) {
    glm::vec3 halfSize = aabb.halfSize();
    glm::vec3 center = aabb.center();
    float r = fabs(halfSize.x * plane.normal.x) + fabs(halfSize.y * plane.normal.y) + fabs(halfSize.z * plane.normal.z);
    float d = glm::dot(plane.normal, center) + plane.distance;
    if (fabs(d) < r) {
        return 0.0;
    }
    else if (d < 0.0) {
        return d + r;
    }
    return d - r;
}

std::tuple<bool, float, glm::vec3> collide(const AABB &s, const AABB &m, const glm::vec3 &vel) {

    float tfirst = 0.0f;
    float tlast = 1.0f;
    float tfirstArr[3] = {-1, -1, -1};
    for(int i = 0; i < 3; i++) {
        if(vel[i] < 0.0f) {
            if(m.max[i] < s.min[i]) return {false, {}, {}}; // Nonintersecting and moving apart
            if(s.max[i] < m.min[i]) {
                tfirstArr[i] = (s.max[i] - m.min[i]) / vel[i];
                tfirst = std::max(tfirstArr[i], tfirst);
            }
            if(s.min[i] < m.max[i]) tlast = std::min((s.min[i] - m.max[i]) / vel[i], tlast);
        }
        else if(vel[i] > 0.0f) {
            if(m.min[i] > s.max[i]) return {false, {}, {}};  // Nonintersecting and moving apart
            if(s.min[i] > m.max[i]) {
                tfirstArr[i] = (s.min[i] - m.max[i]) / vel[i];
                tfirst = std::max(tfirstArr[i], tfirst);
            }
            if(s.max[i] > m.min[i]) tlast = std::min((s.max[i] - m.min[i]) / vel[i], tlast);

        }
        else if(!(m.max[i] > s.min[i] && m.min[i] < s.max[i])) {
            return {false, {}, {}};
        }
        if (tfirst > tlast) return {false, {}, {}};

    }

    int nx = (tfirst == tfirstArr[0]) ? (vel.x > 0 ? -1 : 1) : 0;
    int ny = (tfirst == tfirstArr[1]) ? (vel.y > 0 ? -1 : 1) : 0;
    int nz = (tfirst == tfirstArr[2]) ? (vel.z > 0 ? -1 : 1) : 0;

    // edge case, happens then the boxes touch like this
    /*
     *
     *  -----
     * |     |
     *  -----
     *        -----
     *       |     |
     *        -----
     */
    if (nx == 0.0f && ny == 0.0f && nz == 0.0f) return {false, {}, {}};

    return {true, tfirst, glm::vec3{nx, ny, nz}};

}