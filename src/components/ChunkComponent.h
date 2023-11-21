//
// Created by Phil Lan on 2023-11-03.
//

#ifndef MINECRAFT_CLONE_CHUNKCOMPONENT_H
#define MINECRAFT_CLONE_CHUNKCOMPONENT_H
#include <glm/glm.hpp>
#include <memory>
#include <array>
#include "BlockType.h"
#include "vector_extensions.h"

#define CHUNK_LENGTH 16
#define CHUNK_HEIGHT 256
using BlockArray = std::array<BlockType, CHUNK_HEIGHT * CHUNK_LENGTH * CHUNK_LENGTH>;
struct ChunkComponent {
    bool modified = true;
    std::unique_ptr<BlockArray> blockTypes;

    ChunkComponent(BlockType b);

    BlockType getBlock(const glm::ivec3 &localCoords) const;
    void setBlock(const glm::ivec3 &localCoords, BlockType blockType);
};

constexpr glm::ivec2 toChunk(const glm::vec3 &coords) {
    return {floor(coords.x / CHUNK_LENGTH), floor(coords.z / CHUNK_LENGTH)};
}

constexpr glm::ivec3 toLocal(const glm::vec3 &coords) {
    const glm::ivec2 chunkCoords = toChunk(coords);
    return {floor(coords.x) - chunkCoords.x * CHUNK_LENGTH, coords.y, floor(coords.z) - chunkCoords.y * CHUNK_LENGTH};
}
#endif //MINECRAFT_CLONE_CHUNKCOMPONENT_H
