//
// Created by Phil Lan on 2023-10-30.
//
#include "TransformationComponent.h"
#include <glm/gtx/transform.hpp>

void TransformationComponent::calculateTransform() {
    glm::mat4 rotation = glm::toMat4(this->rotation);

    transform = glm::translate(position) * glm::scale(scale) * rotation;
}

TransformationComponent::TransformationComponent(const glm::vec3 &position, const glm::quat &rotation,
                                                 const glm::vec3 &scale):
    position(position),
    rotation(rotation),
    scale(scale) {
    calculateTransform();
}
