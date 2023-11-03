//
// Created by Phil Lan on 2023-10-27.
//

#include "RenderingSystem.h"
#include "components/MeshComponent.h"
#include "components/TransformationComponent.h"
#include "components/CameraComponent.h"

void RenderingSystem::update(float dt) {
    shader->use();
    registry.view<MeshComponent, TransformationComponent>().each([&](const MeshComponent& meshComponent, const TransformationComponent& transform) {
        shader->setMatrix4("modelMatrix", transform.transform);
        meshComponent.mesh.render(shader);
    });
}

RenderingSystem::RenderingSystem(entt::registry& registry, entt::dispatcher& dispatcher, InputManager& inputManager) :
    System(registry, dispatcher, inputManager),
    shader(new Shader(ASSETS_PATH"/shaders/vert.glsl", ASSETS_PATH"/shaders/frag.glsl")) {
    dispatcher.sink<CameraUpdateEvent>().connect<&RenderingSystem::onCameraUpdate>(*this);
}

void RenderingSystem::onCameraUpdate(CameraUpdateEvent &event) {
    const CameraComponent& cameraComponent = registry.get<CameraComponent>(event.entity);
    shader->setMatrix4("projectionViewMatrix", cameraComponent.projectionMatrix * cameraComponent.viewMatrix);
}
