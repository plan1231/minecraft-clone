//
// Created by Phil Lan on 2023-11-11.
//
#include "ChunkComponent.h"

ChunkComponent::ChunkComponent(BlockType b) {
    blockTypes = std::make_unique<BlockArray>();
    blockTypes->fill(b);
}

BlockType ChunkComponent::getBlock(const glm::ivec3 &localCoords) const {
    return (*blockTypes)[localCoords.y * CHUNK_LENGTH * CHUNK_LENGTH + localCoords.x * CHUNK_LENGTH + localCoords.z];
}

void ChunkComponent::setBlock(const glm::ivec3& localCoords, BlockType blockType) {
    (*blockTypes)[localCoords.y * CHUNK_LENGTH * CHUNK_LENGTH + localCoords.x * CHUNK_LENGTH + localCoords.z] = blockType;
}
