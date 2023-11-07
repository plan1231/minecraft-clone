//
// Created by Phil Lan on 2023-10-30.
//

#include "PlayerSystem.h"
#include "events/CameraUpdateEvent.h"
#include "components/TransformComponent.h"
#include "components/CameraComponent.h"
#include <iostream>
#include <format>
PlayerSystem::PlayerSystem(entt::registry &registry, entt::dispatcher &dispatcher, InputManager &inputManager) :
        System(registry, dispatcher, inputManager) {
    playerEntity = registry.create();
    TransformComponent &t = registry.emplace<TransformComponent>(playerEntity, glm::vec3(0.0f, 0.0f, 0.0f),
                                                                 glm::quat(),
                                                                 glm::vec3(1.0f, 1.0f, 1.0f));
    registry.emplace<CameraComponent>(playerEntity).calculateMatrices(t);
}

void PlayerSystem::update(float dt) {
    CameraComponent &cameraComponent = registry.get<CameraComponent>(playerEntity);
    TransformComponent &transformComponent = registry.get<TransformComponent>(playerEntity);

    static glm::dvec2 lastCursorPos = inputManager.getCursorPos();
    glm::dvec2 cursorPos = inputManager.getCursorPos();
    glm::dvec2 cursorChange = cursorPos - lastCursorPos;

    bool rotationChanged = false;
    if(cursorChange != glm::dvec2()) {
        rotationChanged = true;
        cameraComponent.yaw += cursorChange.x * 0.05;
        cameraComponent.pitch -= cursorChange.y * 0.05;

        cameraComponent.pitch = glm::clamp(cameraComponent.pitch, -89.0f, 89.0f);

        if (cameraComponent.yaw >= 360.0f) cameraComponent.yaw -= 360.0f;
        else if (cameraComponent.yaw <= -360.0f) cameraComponent.yaw -= 360.0f;
    }

    const glm::vec3 playerW = cameraComponent.front;
    const glm::vec3 playerR = cameraComponent.right;

    glm::vec3 velocity{};
    if(inputManager.getKey(GLFW_KEY_W) == GLFW_PRESS)
        velocity += cameraComponent.front;
    if(inputManager.getKey(GLFW_KEY_S) == GLFW_PRESS)
        velocity -= cameraComponent.front;
    if(inputManager.getKey(GLFW_KEY_D) == GLFW_PRESS)
        velocity += cameraComponent.right;
    if(inputManager.getKey(GLFW_KEY_A) == GLFW_PRESS)
        velocity -= cameraComponent.right;

    bool positionChanged = false;
    if(velocity != glm::vec3{}) {
        transformComponent.position += glm::normalize(velocity) * dt * 3.0f;
        positionChanged = true;
        std::cout << transformComponent.position.x << " y " << transformComponent.position.y << " z " << transformComponent.position.z << std::endl;
    }

    if(rotationChanged || positionChanged) {
        cameraComponent.calculateMatrices(transformComponent);
        dispatcher.trigger<CameraUpdateEvent>(CameraUpdateEvent{playerEntity});
    }
    lastCursorPos = cursorPos;
}
