//
// Created by Phil Lan on 2023-11-10.
//

#ifndef MINECRAFT_CLONE_UTIL_H
#define MINECRAFT_CLONE_UTIL_H
#include <glm/glm.hpp>

/**
 * \brief Get the manhattan distance between two positions
 * \param a position vector a
 * \param b position vector b
 * \return manhattan distance between a and b
 */
constexpr unsigned int manhattan(const glm::ivec2 &a, const glm::ivec2 &b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

/**
 * \brief component-wise floor division (round to negative infinity)
 * \param v vector
 * \param d denominator
 * \return original vector with each component floor-divided
 */
constexpr glm::ivec3 fdiv(const glm::ivec3 &v, int d) {
    glm::ivec3 res;

    res.x = v.x / d - (v.x % d < 0);
    res.y = v.y / d - (v.y % d < 0);
    res.z = v.z / d - (v.z % d < 0);

    return res;
}

/**
 * \brief Positive component-wise mod
 * \param v vector
 * \param m number to mod each component by
 * \return v % m, but each component t statisfies 0 <= t < m
 */
constexpr glm::ivec3 pmod(glm::ivec3 v, int m) {
    glm::ivec3 res;

    res.x =  ((v.x %= m) < 0) ? v.x + m : v.x;
    res.y =  ((v.y %= m) < 0) ? v.y + m : v.y;
    res.z =  ((v.z %= m) < 0) ? v.z + m : v.z;

    return res;
}

/**
 * \brief Round down a vector of ints to vector of floats.
 * \param
 * \return
 */
constexpr glm::ivec3 floor(const glm::vec3 &v) {
    glm::ivec3 res;

    res.x = floor(v.x);
    res.y = floor(v.y);
    res.z = floor(v.z);

    return res;
}
#endif //MINECRAFT_CLONE_UTIL_H
