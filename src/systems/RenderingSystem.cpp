//
// Created by Phil Lan on 2023-10-27.
//

#include "RenderingSystem.h"
#include "components/MeshComponent.h"
#include "components/TransformComponent.h"
#include "components/CameraComponent.h"

void RenderingSystem::update(float dt) {
    shader->use();
    registry.view<MeshComponent, TransformComponent>().each([&](const MeshComponent& meshComponent, const TransformComponent& transform) {
        shader->setMatrix4("modelMatrix", transform.transform);
        meshComponent.render(shader);
    });
}

RenderingSystem::RenderingSystem(entt::registry& registry, entt::dispatcher& dispatcher) :
    System(registry, dispatcher),
    shader(new Shader(ASSETS_PATH"/shaders/chunk.vert.glsl", ASSETS_PATH"/shaders/chunk.frag.glsl")) {
    dispatcher.sink<CameraUpdateEvent>().connect<&RenderingSystem::onCameraUpdate>(*this);
}

void RenderingSystem::onCameraUpdate(CameraUpdateEvent &event) {
    const CameraComponent& cameraComponent = registry.get<CameraComponent>(event.entity);
    shader->setMatrix4("projectionViewMatrix", cameraComponent.projectionMatrix * cameraComponent.viewMatrix);
}
