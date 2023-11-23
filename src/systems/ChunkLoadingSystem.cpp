//
// Created by Phil Lan on 2023-11-05.
//

#include "ChunkLoadingSystem.h"

#include "components/CameraComponent.h"
#include "components/TransformComponent.h"
const int LOAD_DISTANCE = 3;

ChunkLoadingSystem::ChunkLoadingSystem(entt::registry& registry, entt::dispatcher& dispatcher):
    System(registry, dispatcher)
{

}

void ChunkLoadingSystem::update(float dt) {
    static int tickCount = 0;
    tickCount++;
    if(tickCount == 100) {
        tickCount = 0;
    }
    else return;
    registry.view<TransformComponent, CameraComponent>().each([&](TransformComponent &transform, CameraComponent &camera) {
        glm::ivec2 chunkCoords = toChunk(transform.position);
        for(int x = chunkCoords.x - LOAD_DISTANCE; x <= chunkCoords.x + LOAD_DISTANCE; x++) {
            for(int z = chunkCoords.y - LOAD_DISTANCE; z <= chunkCoords.y + LOAD_DISTANCE; z++) {
                if(chunkManager.chunks.contains({x, z})) continue;
                ChunkComponent &cc = chunkManager.loadChunk({x, z}, BlockType::AIR);
                terrainGen.generateTerrain(cc, {x, z});
            }
        }
    });
}
