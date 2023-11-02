//
// Created by Phil Lan on 2023-10-30.
//

#include "CameraSystem.h"
#include "events/CameraUpdateEvent.h"
#include "components/TransformationComponent.h"
#include "components/CameraComponent.h"

CameraSystem::CameraSystem(entt::registry& registry, entt::dispatcher& dispatcher, InputManager& inputManager):
    System(registry, dispatcher, inputManager) {
    cameraEntity = registry.create();
    TransformationComponent& t = registry.emplace<TransformationComponent>(cameraEntity, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(),
                                              glm::vec3(1.0f, 1.0f, 1.0f));
    registry.emplace<CameraComponent>(cameraEntity).calculateMatrices(t);
}

void CameraSystem::update(float dt) {
     CameraComponent& cameraComponent = registry.get<CameraComponent>(cameraEntity);
     TransformationComponent& transformationComponent = registry.get<TransformationComponent>(cameraEntity);
     cameraComponent.yaw += 5 * 0.1;
     if (cameraComponent.yaw >= 360) cameraComponent.yaw -= 360;
     cameraComponent.calculateMatrices(transformationComponent);
     dispatcher.trigger<CameraUpdateEvent>(CameraUpdateEvent{cameraEntity});
}
