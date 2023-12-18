//
// Created by Phil Lan on 2023-11-11.
//
#include "Chunk.h"


BlockType Chunk::getBlock(const glm::ivec3 &localCoords) const {
    return blockTypes[localCoords.y * CHUNK_ACTUAL_SIZE * CHUNK_ACTUAL_SIZE + localCoords.x * CHUNK_ACTUAL_SIZE + localCoords.z];
}

void Chunk::setBlock(const glm::ivec3& localCoords, BlockType blockType) {
    blockTypes[localCoords.y * CHUNK_ACTUAL_SIZE * CHUNK_ACTUAL_SIZE + localCoords.x * CHUNK_ACTUAL_SIZE + localCoords.z] = blockType;
    modified = true;
}
