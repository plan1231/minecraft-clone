//
// Created by Phil Lan on 2023-10-27.
//

#include "RenderingSystem.h"

#include "GameEntities.h"
#include "components/Model.h"
#include "components/Transform.h"
#include "components/Camera.h"
#include "components/Chunk.h"
#include "components/Crosshair.h"
#include "components/TempMesh.h"

void RenderingSystem::update(float dt) {
    chunkShader->use();
    entt::entity player = entt::locator<GameEntities>::value().player;

    const auto [camera, pTrans] = registry.get<Camera, Transform>(player);
    camera.calculateMatrices(pTrans);
    camera.calculateFrustrum();
    chunkShader->setMatrix4("projectionViewMatrix", camera.pvMatrix);

    registry.view<Model, TempMesh<BlockVertex>, Chunk>().each([](Model &model, TempMesh<BlockVertex> &tmpMesh, Chunk &chunk) {
        if(!chunk.modified)  return;
        model.mesh->bufferData(tmpMesh.vertices.data(), tmpMesh.vertices.size() * sizeof(BlockVertex),
                                     tmpMesh.indices, GL_STATIC_DRAW);
        chunk.modified = false;
    });


    registry.view<Model, Transform>().each([&](auto entity, const Model &model, const Transform &transform) {

        // Frustrum culling for chunks
        if(registry.all_of<Chunk>(entity)) {
            Camera &camera = registry.get<Camera>(player);

            AABB cAABB{
                .min = transform.position,
                .max = transform.position + glm::vec3{CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE}
            };
            if(!camera.frustrum.contains(cAABB)) return;
        }

        chunkShader->setMatrix4("modelMatrix", transform.transform);
        model.render(chunkShader);
    });

    lineShader->use();

    registry.view<Model, Crosshair>().each([&](auto entity, const Model &model, const Crosshair &crosshair) {
        lineShader->setMatrix4("projectionMatrix", crosshair.projectionMatrix);
        model.render(lineShader);

    });
}

RenderingSystem::RenderingSystem(entt::registry& registry, entt::dispatcher& dispatcher) :
    System(registry, dispatcher),
    chunkShader{std::make_shared<Shader>(ASSETS_PATH"/shaders/chunk.vert.glsl", ASSETS_PATH"/shaders/chunk.frag.glsl")},
    lineShader{std::make_shared<Shader>(ASSETS_PATH"/shaders/line.vert.glsl", ASSETS_PATH"/shaders/line.frag.glsl")}
{
    glLineWidth(CROSSHAIR_THICKNESS);

}

void RenderingSystem::onCameraUpdate(CameraUpdateEvent &event) {

}
