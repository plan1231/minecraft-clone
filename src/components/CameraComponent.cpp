//
// Created by Phil Lan on 2023-10-30.
//

#include "CameraComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
void CameraComponent::calculateVectors() {
    front = glm::vec3(
        glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw)),
        glm::sin(glm::radians(pitch)),
        glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw))
    );
    constexpr glm::vec3 up {0.0f, 1.0f, 0.0f};
    right = glm::cross(up, front);

    this->up = glm::normalize(glm::cross(right, front));
}

void CameraComponent::calculateMatrices(const TransformationComponent &transform) {
    projectionMatrix = glm::perspective(glm::radians(fov), width / height, near, far);

    calculateVectors();
    glm::vec3 cameraTarget = transform.position + front;
    viewMatrix = glm::lookAt(transform.position, cameraTarget, up);
}
