//
// Created by Phil Lan on 2023-10-30.
//

#include "CameraSystem.h"
#include "events/CameraUpdateEvent.h"
#include "components/TransformComponent.h"
#include "components/CameraComponent.h"
#include <iostream>
#include <format>
CameraSystem::CameraSystem(entt::registry &registry, entt::dispatcher &dispatcher, InputManager &inputManager) :
        System(registry, dispatcher, inputManager) {
    cameraEntity = registry.create();
    TransformComponent &t = registry.emplace<TransformComponent>(cameraEntity, glm::vec3(0.0f, 0.0f, 0.0f),
                                                                           glm::quat(),
                                                                           glm::vec3(1.0f, 1.0f, 1.0f));
    registry.emplace<CameraComponent>(cameraEntity).calculateMatrices(t);
}

void CameraSystem::update(float dt) {
    CameraComponent &cameraComponent = registry.get<CameraComponent>(cameraEntity);
    TransformComponent &transformationComponent = registry.get<TransformComponent>(cameraEntity);

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
        else if (cameraComponent.yaw <= -360.0f) cameraComponent.yaw += 360.0f;
        std::cout << "yaw: " << cameraComponent.yaw << "\t pitch: " << cameraComponent.pitch << std::endl;
    }

    if(rotationChanged) {
        cameraComponent.calculateMatrices(transformationComponent);
        dispatcher.trigger<CameraUpdateEvent>(CameraUpdateEvent{cameraEntity});
    }
    lastCursorPos = cursorPos;
}
