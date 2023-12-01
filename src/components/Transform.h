//
// Created by Phil Lan on 2023-10-29.
//

#ifndef MINECRAFT_CLONE_TRANSFORMCOMPONENT_H
#define MINECRAFT_CLONE_TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform {
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;

    glm::mat4 transform;

    void calculateTransform();
    Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);
};
#endif //MINECRAFT_CLONE_TRANSFORMCOMPONENT_H
