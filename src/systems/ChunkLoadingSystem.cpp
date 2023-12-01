//
// Created by Phil Lan on 2023-11-05.
//

#include "ChunkLoadingSystem.h"

#include "components/Camera.h"
#include "components/Transform.h"

ChunkLoadingSystem::ChunkLoadingSystem(entt::registry& registry, entt::dispatcher& dispatcher):
    System(registry, dispatcher)
{

}

void ChunkLoadingSystem::update(float dt) {
    static int tickCount = 99;
    tickCount++;
    if(tickCount == 100) {
        tickCount = 0;
    }
    else return;
    registry.view<Transform, Camera>().each([&](Transform &transform, Camera &camera) {
        glm::ivec2 chunkCoords = toChunk(transform.position);
        for(int x = chunkCoords.x - LOAD_DISTANCE; x <= chunkCoords.x + LOAD_DISTANCE; x++) {
            for(int z = chunkCoords.y - LOAD_DISTANCE; z <= chunkCoords.y + LOAD_DISTANCE; z++) {
                if(chunkManager.chunks.contains({x, z})) continue;
                Chunk &cc = chunkManager.loadChunk({x, z}, BlockType::AIR);
                terrainGen.generateTerrain(cc, {x, z});
            }
        }
    });
}
