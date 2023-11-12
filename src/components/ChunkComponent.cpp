//
// Created by Phil Lan on 2023-11-11.
//
#include "ChunkComponent.h"

ChunkComponent::ChunkComponent(BlockType b) {
    blockTypes = std::make_unique<BlockArray>();
    blockTypes->fill(b);
}

BlockType ChunkComponent::getBlock(const glm::ivec3 &localCoords) const {
    return (*blockTypes)[localCoords.x * CHUNK_LENGTH * CHUNK_LENGTH + localCoords.y * CHUNK_LENGTH + localCoords.z];
}
