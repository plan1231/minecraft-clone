//
// Created by Phil Lan on 2023-11-03.
//

#include "ChunkManager.h"

#include "../factories.h"
#include "../components/Model.h"
#include "../components/Transform.h"

BlockType ChunkManager::getBlock(const glm::ivec3 &coords) {
    if (coords.y >= MAX_Y) return BlockType::AIR;
    glm::ivec3 chunkCoords = toChunk(coords);
    auto it = chunks.find(chunkCoords);
    if (it == chunks.end()) {
        return BlockType::AIR;
    }
    Chunk &chunk = it->second;
    glm::ivec3 localCoords = toLocal(coords);
    return chunk.getBlock(localCoords);
}

void ChunkManager::setBlock(const glm::ivec3 &coords, BlockType blockType) {
    glm::ivec3 chunkCoords = toChunk(coords);
    glm::ivec3 localCoords = toLocal(coords);
    auto it = chunks.find(chunkCoords);
    if (it == chunks.end()) {
        return;
    }
    Chunk &chunk = it->second;
    chunk.setBlock(localCoords, blockType);

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

ChunkManager::ChunkManager() {
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
    return &it->second;
}

Chunk& ChunkManager::loadChunk(const glm::ivec3 &chunkCoords) {
    // todo: Make resource loading more sane
    static TexturePtr chunkTexture = std::make_shared<Texture>(ASSETS_PATH"/textures/blocks.png");

    MeshPtr meshPtr = std::make_shared<Mesh>(VertexAttributes{
            VertexAttribute{3, GL_FLOAT, GL_FALSE, 5 * sizeof(float) + 1 * sizeof(unsigned int), 0},
            VertexAttribute{
                    2, GL_FLOAT, GL_FALSE, 5 * sizeof(float) + 1 * sizeof(unsigned int),
                    (void *)(3 * sizeof(float))
            },
            VertexAttribute{1, GL_FLOAT, GL_FALSE, 5 * sizeof(float) + 1 * sizeof(unsigned int),
                            (void *)(5 * sizeof(float))
            }
    }, GL_TRIANGLES);


    chunks.emplace(chunkCoords, Chunk{ .model{ meshPtr, chunkTexture}});
    return chunks[chunkCoords];
}
