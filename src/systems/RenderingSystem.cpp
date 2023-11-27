//
// Created by Phil Lan on 2023-10-27.
//

#include "RenderingSystem.h"

#include "GameEntities.h"
#include "components/MeshComponent.h"
#include "components/TransformComponent.h"
#include "components/CameraComponent.h"
#include "components/ChunkComponent.h"

void RenderingSystem::update(float dt) {
    shader->use();
    entt::entity player = entt::locator<GameEntities>::value().player;

    registry.view<MeshComponent, TransformComponent>().each([&](auto entity, const MeshComponent& meshComponent, const TransformComponent& transform) {

        // Frustrum culling for chunks
        if(registry.all_of<ChunkComponent>(entity)) {
            CameraComponent &camera = registry.get<CameraComponent>(player);
            glm::vec3 halfLength = {CHUNK_LENGTH / 2, CHUNK_HEIGHT / 2, CHUNK_LENGTH};
            AABB cAABB(halfLength, halfLength + transform.position);
            if(!camera.frustrum.contains(cAABB)) return;
        }

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
    shader->setMatrix4("projectionViewMatrix", cameraComponent.pvMatrix);
}
