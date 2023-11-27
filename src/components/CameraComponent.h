//
// Created by Phil Lan on 2023-10-30.
//

#ifndef MINECRAFT_CLONE_CAMERACOMPONENT_H
#define MINECRAFT_CLONE_CAMERACOMPONENT_H
#include <glm/glm.hpp>
#include "TransformComponent.h"
#include "geometry/Frustrum.h"
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
    glm::mat4 pvMatrix;
    Frustrum frustrum;

    /**
     * \brief Calculate the projection and view matrices
     * \param transform represents the position of the camera
     */
    void calculateMatrices(const TransformComponent& transform);

    /**
     * @brief Calculate the up, front, right vectors
     */
    void calculateVectors();

    /**
     * \brief Calculate the frustrum based on the projection and view matricess
     */
    void calculateFrustrum();
};


#endif //MINECRAFT_CLONE_CAMERACOMPONENT_H
