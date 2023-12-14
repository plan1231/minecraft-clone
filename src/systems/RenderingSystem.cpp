//
// Created by Phil Lan on 2023-10-27.
//

#include "RenderingSystem.h"

#include <glm/gtx/transform.hpp>
#include "GameEntities.h"
#include "components/Model.h"
#include "components/Transform.h"
#include "components/Camera.h"
#include "world/Chunk.h"
#include "components/Crosshair.h"
#include "components/TempMesh.h"

void RenderingSystem::update(float dt) {
    chunkShader->use();
    entt::entity player = entt::locator<GameEntities>::value().player;

    const auto [camera, pTrans] = registry.get<Camera, Transform>(player);
    camera.calculateMatrices(pTrans);
    camera.calculateFrustrum();
    chunkShader->setMatrix4("projectionViewMatrix", camera.pvMatrix);

    // upload updated chunk mesh to the gpu
    for(auto& [chunkCoords, chunk] : chunkManager.chunks) {
        if (!chunk.modified) continue;
        chunk.model.mesh->bufferData(chunk.tempMesh.vertices.data(), chunk.tempMesh.vertices.size() * sizeof(BlockVertex),
                               chunk.tempMesh.indices, GL_STATIC_DRAW);
        chunk.modified = false;
        chunk.tempMesh.indices.clear();
        chunk.tempMesh.vertices.clear();
    }

    for(auto& [chunkCoords, chunk] : chunkManager.chunks) {
        // Frustrum culling for chunks
        Camera &camera = registry.get<Camera>(player);
        glm::vec3 chunkGlobalCoords = chunkCoords * CHUNK_SIZE;
        AABB cAABB{
                .min = chunkGlobalCoords,
                .max = chunkGlobalCoords + glm::vec3{CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE}
        };
        if (!camera.frustrum.contains(cAABB)) continue;

        chunkShader->setMatrix4("modelMatrix", glm::translate(chunkGlobalCoords));
        chunk.model.render(chunkShader);
    }

    lineShader->use();

    registry.view<Model, Crosshair>().each([&](auto entity, const Model &model, const Crosshair &crosshair) {
        lineShader->setMatrix4("projectionMatrix", crosshair.projectionMatrix);
        model.render(lineShader);

    });
}

RenderingSystem::RenderingSystem(entt::registry &registry, entt::dispatcher &dispatcher) :
        System(registry, dispatcher),
        chunkShader{
                std::make_shared<Shader>(ASSETS_PATH"/shaders/chunk.vert.glsl", ASSETS_PATH"/shaders/chunk.frag.glsl")},
        lineShader{
                std::make_shared<Shader>(ASSETS_PATH"/shaders/line.vert.glsl", ASSETS_PATH"/shaders/line.frag.glsl")} {
    glLineWidth(CROSSHAIR_THICKNESS);

}

void RenderingSystem::onCameraUpdate(CameraUpdateEvent &event) {

}
