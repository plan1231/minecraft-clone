//
// Created by Phil Lan on 2023-10-29.
//

#ifndef MINECRAFT_CLONE_TRANSFORMATIONCOMPONENT_H
#define MINECRAFT_CLONE_TRANSFORMATIONCOMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct TransformationComponent {
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;

    glm::mat4 transform;

    void calculateTransform();
    TransformationComponent(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);
};
#endif //MINECRAFT_CLONE_TRANSFORMATIONCOMPONENT_H
