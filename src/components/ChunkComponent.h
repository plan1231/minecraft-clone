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
using BlockArray = std::array<BlockType, CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_LENGTH>;
struct ChunkComponent {
    bool modified = true;
    std::unique_ptr<BlockArray> blockTypes;
    ChunkComponent(BlockType b);
    BlockType getBlock(const glm::ivec3 &localCoords) const;
};


constexpr glm::ivec3 toChunk(const glm::ivec3 &coords) {
    return fdiv(coords, CHUNK_LENGTH);
}

constexpr glm::ivec3 toChunk(const glm::vec3 &coords) {
    return floor(coords / static_cast<float>(CHUNK_LENGTH));
}

constexpr glm::ivec3 toLocal(const glm::ivec3 &coords) {
    return pmod(coords, CHUNK_LENGTH);
}
constexpr glm::ivec3 toLocal(const glm::vec3 coords) {
    return pmod(floor(coords), CHUNK_LENGTH);
}

#endif //MINECRAFT_CLONE_CHUNKCOMPONENT_H
