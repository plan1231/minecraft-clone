//
// Created by Phil Lan on 2023-11-03.
//

#include "ChunkManager.h"

#include "Application.h"
#include "Application.h"
#include "components/Model.h"
#include "components/Transform.h"
#include "components/Chunk.h"
#include "rendering/Mesh.h"
#include "vector_extensions.h"

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
    component.modified = true;
}

::Chunk& ChunkManager::loadChunk(const glm::ivec3 &chunkCoords, BlockType b) {
    Mesh* g = new Mesh(VertexAttributes{
        VertexAttribute{3, GL_FLOAT, GL_FALSE, 5 * sizeof(float) + 1 * sizeof(unsigned int), 0},
        VertexAttribute{
            2, GL_FLOAT, GL_FALSE, 5 * sizeof(float) + 1 * sizeof(unsigned int),
            (void *)(3 * sizeof(float))
        },
        VertexAttribute{1, GL_FLOAT, GL_FALSE, 5 * sizeof(float) + 1 * sizeof(unsigned int),
            (void *)(5 * sizeof(float))
        }
    }, GL_TRIANGLES);

    Texture* t = new Texture(ASSETS_PATH"/textures/blocks.png");
    Model m{g, t};
    entt::entity chunk = registry.create();
    registry.emplace<Chunk>(chunk, Chunk(b));
    registry.emplace<Model>(chunk, m);
    registry.emplace<Transform>(chunk, glm::vec3{chunkCoords.x * CHUNK_SIZE, chunkCoords.y * CHUNK_SIZE, chunkCoords.z * CHUNK_SIZE},
                                glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
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
