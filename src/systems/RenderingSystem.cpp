//
// Created by Phil Lan on 2023-10-27.
//

#include "RenderingSystem.h"

#include "GameEntities.h"
#include "components/Model.h"
#include "components/Transform.h"
#include "components/Camera.h"
#include "components/Chunk.h"

void RenderingSystem::update(float dt) {
    shader->use();
    entt::entity player = entt::locator<GameEntities>::value().player;

    const auto [camera, pTrans] = registry.get<Camera, Transform>(player);
    camera.calculateMatrices(pTrans);
    camera.calculateFrustrum();
    shader->setMatrix4("projectionViewMatrix", camera.pvMatrix);

    registry.view<Model, Transform>().each([&](auto entity, const Model& meshComponent, const Transform& transform) {

        // Frustrum culling for chunks
        if(registry.all_of<Chunk>(entity)) {
            Camera &camera = registry.get<Camera>(player);

            AABB cAABB{
                .min = transform.position,
                .max = transform.position + glm::vec3{CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE}
            };
            if(!camera.frustrum.contains(cAABB)) return;
        }

        shader->setMatrix4("modelMatrix", transform.transform);
        meshComponent.render(shader);
    });
}

RenderingSystem::RenderingSystem(entt::registry& registry, entt::dispatcher& dispatcher) :
    System(registry, dispatcher),
    shader(new Shader(ASSETS_PATH"/shaders/chunk.vert.glsl", ASSETS_PATH"/shaders/chunk.frag.glsl")) {
}

void RenderingSystem::onCameraUpdate(CameraUpdateEvent &event) {

}
