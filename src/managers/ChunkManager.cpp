//
// Created by Phil Lan on 2023-11-03.
//

#include "ChunkManager.h"

#include "factories.h"
#include "components/Model.h"
#include "components/Transform.h"
#include "components/Chunk.h"

BlockType ChunkManager::getBlock(const glm::ivec3 &coords) {
    if (coords.y >= MAX_Y) return BlockType::AIR;
    glm::ivec3 chunkCoords = toChunk(coords);
    auto it = chunks.find(chunkCoords);
    if (it == chunks.end()) {
        return BlockType::AIR;
    }
    entt::entity entity = it->second;
    Chunk &component = registry.get<Chunk>(entity);
    glm::ivec3 localCoords = toLocal(coords);
    return component.getBlock(localCoords);
}

void ChunkManager::setBlock(const glm::ivec3 &coords, BlockType blockType) {
    glm::ivec3 chunkCoords = toChunk(coords);
    glm::ivec3 localCoords = toLocal(coords);
    auto it = chunks.find(chunkCoords);
    if (it == chunks.end()) {
        return;
    }
    entt::entity entity = it->second;
    Chunk &component = registry.get<Chunk>(entity);
    component.setBlock(localCoords, blockType);

    // also update the neighboring chunks' record of this location
    AdjacentChunks adjChunks = getAdjacent(chunkCoords);

    if(localCoords.x == 1 && adjChunks.prevX) {
        adjChunks.prevX->setBlock({CHUNK_ACTUAL_SIZE - 1, localCoords.y, localCoords.z}, blockType);
    }
    else if(localCoords.x == CHUNK_SIZE && adjChunks.nextX) {
        adjChunks.nextX->setBlock({0, localCoords.y, localCoords.z}, blockType);
    }

    if(localCoords.y == 1 && adjChunks.prevY) {
        adjChunks.prevY->setBlock({localCoords.x, CHUNK_ACTUAL_SIZE - 1, localCoords.z}, blockType);
    }
    else if(localCoords.y == CHUNK_SIZE && adjChunks.nextY) {
        adjChunks.nextY->setBlock({localCoords.x,0, localCoords.z}, blockType);
    }

    if(localCoords.z == 1 && adjChunks.prevZ) {
        adjChunks.prevZ->setBlock({localCoords.x, localCoords.y, CHUNK_ACTUAL_SIZE - 1}, blockType);
    }
    else if(localCoords.z == CHUNK_SIZE && adjChunks.nextZ) {
        adjChunks.nextZ->setBlock({localCoords.x, localCoords.y, 0}, blockType);
    }

}

::Chunk& ChunkManager::loadChunk(const glm::ivec3 &chunkCoords, BlockType b) {
    entt::entity chunk = makeChunk(registry, chunkCoords);
    chunks[chunkCoords] = chunk;
    return registry.get<Chunk>(chunk);
}

ChunkManager::ChunkManager(entt::registry &registry): registry(registry) {
}

AdjacentChunks ChunkManager::getAdjacent(const glm::ivec3 &chunkCoords) {
    return AdjacentChunks{
        .prevX = getChunk({chunkCoords.x - 1, chunkCoords.y, chunkCoords.z}),
        .nextX = getChunk({chunkCoords.x + 1, chunkCoords.y, chunkCoords.z}),

        .prevY = getChunk({chunkCoords.x, chunkCoords.y - 1, chunkCoords.z}),
        .nextY = getChunk({chunkCoords.x, chunkCoords.y + 1, chunkCoords.z}),

        .prevZ = getChunk({chunkCoords.x, chunkCoords.y, chunkCoords.z - 1}),
        .nextZ = getChunk({chunkCoords.x, chunkCoords.y, chunkCoords.z + 1})
    };
}

Chunk* ChunkManager::getChunk(const glm::ivec3 &chunkCoords) {
    auto it = chunks.find(chunkCoords);
    if (it == chunks.end()) {
        return nullptr;
    }
    return &registry.get<Chunk>(it->second);
}
