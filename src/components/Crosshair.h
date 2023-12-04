//
// Created by Phil Lan on 2023-12-03.
//

#ifndef CROSSHAIR_H
#define CROSSHAIR_H
#include <glm/glm.hpp>

// Length of crosshair from the center to one end
constexpr int CROSSHAIR_LENGTH = 10;
constexpr int CROSSHAIR_THICKNESS = 3;
struct Crosshair {
    glm::mat4 projectionMatrix;
};
#endif //CROSSHAIR_H
