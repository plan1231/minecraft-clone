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
    glm::ivec3 localCoords = toLocal(coords);
    glm::ivec3 chunkCoords = toChunk(coords);
    auto it = chunks.find(chunkCoords);
    if (it == chunks.end()) {
        return BlockType::AIR;
    }
    entt::entity entity = it->second;
    ChunkComponent &component = registry.get<ChunkComponent>(entity);
    return component.getBlock(localCoords);
}

void ChunkManager::setBlock(const glm::ivec3 &coords, BlockType blockType) {
    glm::ivec3 localCoords = toLocal(coords);
    glm::ivec3 chunkCoords = toChunk(coords);

    auto it = chunks.find(chunkCoords);
    if (it == chunks.end()) {
        return;
    }
    entt::entity entity = it->second;
    ChunkComponent &component = registry.get<ChunkComponent>(entity);
    (*component.blockTypes)[localCoords.x * CHUNK_LENGTH * CHUNK_LENGTH + localCoords.y * CHUNK_LENGTH + localCoords.z] = blockType;
    component.modified = true;
}

void ChunkManager::loadChunk(const glm::ivec3& chunkCoords, BlockType b) {
    Geometry* g = new Geometry(VertexAttributes{
           VertexAttribute{3, GL_INT, GL_FALSE, 2 * sizeof(float) + 3 * sizeof(unsigned int), 0},
           VertexAttribute{2, GL_FLOAT, GL_FALSE, 2 * sizeof(float) + 3 * sizeof(unsigned int),  (void*)(3 * sizeof(unsigned int))},
       }, GL_TRIANGLES);

    Texture* t = new Texture(ASSETS_PATH"/textures/container.jpg");
    MeshComponent m{g, t};
    entt::entity chunk = registry.create();
    registry.emplace<ChunkComponent>(chunk, ChunkComponent(b));
    registry.emplace<MeshComponent>(chunk, m);
    registry.emplace<TransformComponent>(chunk, chunkCoords * CHUNK_LENGTH, glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
    chunks[chunkCoords] = chunk;
}

ChunkManager::ChunkManager(entt::registry& registry): registry(registry) {
}
