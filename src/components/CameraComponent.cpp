#include "components/CameraComponent.h"

#include "components/TransformComponent.h"

#include <glm/gtc/matrix_transform.hpp>

void CameraComponent::calculateMatrices(const TransformComponent& transform) {
    projectionMatrix = glm::perspective(glm::radians(fov), width / height, near, far);

    calculateVectors();
    glm::vec3 cameraTarget = transform.position + front;

    viewMatrix = glm::lookAt(transform.position, cameraTarget, up);

    pvMatrix = projectionMatrix * viewMatrix;
}

void CameraComponent::calculateVectors() {
    front = glm::vec3(
            cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

    constexpr glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::normalize(glm::cross(front, up));

    this->up = glm::normalize(glm::cross(right, front));
}

void CameraComponent::calculateFrustrum() {

    frustrum.left.normal = {
        pvMatrix[0][3] + pvMatrix[0][0],
        pvMatrix[1][3] + pvMatrix[1][0],
        pvMatrix[2][3] + pvMatrix[2][0]
    };
    frustrum.left.distance = {pvMatrix[3][3] + pvMatrix[3][0]};

    frustrum.right.normal = {
        pvMatrix[0][3] - pvMatrix[0][0],
        pvMatrix[1][3] - pvMatrix[1][0],
        pvMatrix[2][3] - pvMatrix[2][0]
    };
    frustrum.right.distance = {pvMatrix[3][3] - pvMatrix[3][0]};

    frustrum.bottom.normal = {
        pvMatrix[0][3] + pvMatrix[0][1],
        pvMatrix[1][3] + pvMatrix[1][1],
        pvMatrix[2][3] + pvMatrix[2][1]
    };
    frustrum.bottom.distance = {pvMatrix[3][3] + pvMatrix[3][1]};

    frustrum.top.normal = {
        pvMatrix[0][3] - pvMatrix[0][1],
        pvMatrix[1][3] - pvMatrix[1][1],
        pvMatrix[2][3] - pvMatrix[2][1]
    };
    frustrum.top.distance = {pvMatrix[3][3] - pvMatrix[3][1]};

    frustrum.near.normal = {
        pvMatrix[0][3] + pvMatrix[0][2],
        pvMatrix[1][3] + pvMatrix[1][2],
        pvMatrix[2][3] + pvMatrix[2][2]
    };
    frustrum.near.distance = {pvMatrix[3][3] + pvMatrix[3][2]};

    frustrum.far.normal = {
        pvMatrix[0][3] - pvMatrix[0][2],
        pvMatrix[1][3] - pvMatrix[1][2],
        pvMatrix[2][3] - pvMatrix[2][2]
    };
    frustrum.far.distance = {pvMatrix[3][3] - pvMatrix[3][2]};

    for(auto &plane : frustrum.planes) {
        float length = glm::length(plane.normal);
        plane.normal /= length;
        plane.distance /= length;
    }
}
