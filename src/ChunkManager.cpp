//
// Created by Phil Lan on 2023-11-03.
//

#include "ChunkManager.h"
#include "components/MeshComponent.h"
#include "components/TransformComponent.h"
#include "components/ChunkComponent.h"
#include "rendering/Geometry.h"
#include "vector_extensions.h"
BlockType ChunkManager::getBlock(const glm::ivec3 &coords) {
    glm::ivec2 chunkCoords = toChunk(coords);
    auto it = chunks.find(chunkCoords);
    if (it == chunks.end()) {
        return BlockType::AIR;
    }
    entt::entity entity = it->second;
    ChunkComponent &component = registry.get<ChunkComponent>(entity);
    return component.getBlock(coords);
}

void ChunkManager::setBlock(const glm::ivec3 &coords, BlockType blockType) {
    glm::ivec2 chunkCoords = toChunk(coords);
    glm::ivec3 localCoords = toLocal(coords);
    auto it = chunks.find(chunkCoords);
    if (it == chunks.end()) {
        return;
    }
    entt::entity entity = it->second;
    ChunkComponent &component = registry.get<ChunkComponent>(entity);
    component.setBlock(localCoords, blockType);
    component.modified = true;
}

void ChunkManager::loadChunk(const glm::ivec2 &chunkCoords, BlockType b) {
    Geometry* g = new Geometry(VertexAttributes{
           VertexAttribute{3, GL_INT, GL_FALSE, 2 * sizeof(float) + 3 * sizeof(unsigned int), 0},
           VertexAttribute{2, GL_FLOAT, GL_FALSE, 2 * sizeof(float) + 3 * sizeof(unsigned int),  (void*)(3 * sizeof(unsigned int))},
       }, GL_TRIANGLES);

    Texture* t = new Texture(ASSETS_PATH"/textures/blocks.png");
    MeshComponent m{g, t};
    entt::entity chunk = registry.create();
    registry.emplace<ChunkComponent>(chunk, ChunkComponent(b));
    registry.emplace<MeshComponent>(chunk, m);
    registry.emplace<TransformComponent>(chunk, glm::vec3{chunkCoords.x * CHUNK_LENGTH, 0.0f, chunkCoords.y * CHUNK_LENGTH}, glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
    chunks[chunkCoords] = chunk;
}

ChunkManager::ChunkManager(entt::registry& registry): registry(registry) {
}

AdjacentChunks ChunkManager::getAdjacent(const glm::ivec2& chunkCoords) {
    return AdjacentChunks {
        .prevX = getChunk({chunkCoords.x - 1, chunkCoords.y}),
        .nextX = getChunk({chunkCoords.x + 1, chunkCoords.y}),
        .prevZ = getChunk({chunkCoords.x, chunkCoords.y - 1}),
        .nextZ = getChunk({chunkCoords.x, chunkCoords.y + 1})
    };
}

ChunkComponent* ChunkManager::getChunk(const glm::ivec2& chunkCoords) {
    auto it = chunks.find(chunkCoords);
    if (it == chunks.end()) {
        return nullptr;
    }
    return &registry.get<ChunkComponent>(it->second);
}
