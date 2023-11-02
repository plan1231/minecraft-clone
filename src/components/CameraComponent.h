//
// Created by Phil Lan on 2023-10-30.
//

#ifndef MINECRAFT_CLONE_CAMERACOMPONENT_H
#define MINECRAFT_CLONE_CAMERACOMPONENT_H
#include <glm/glm.hpp>
#include "TransformationComponent.h"

struct CameraComponent {
    glm::vec3 front, right, up;
    float width = 800.0f;
    float height = 600.0f;
    float fov = 60.0f;
    float near = 0.1f;
    float far = 200.0f;

    float yaw = 0.0f, pitch = 0.0f;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    void calculateMatrices(const TransformationComponent& transform);

    /**
     * @brief Calculate the up, front, right vectors
     */
    void calculateVectors();
};


#endif //MINECRAFT_CLONE_CAMERACOMPONENT_H
