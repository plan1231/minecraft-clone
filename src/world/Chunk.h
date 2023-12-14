//
// Created by Phil Lan on 2023-11-03.
//

#ifndef MINECRAFT_CLONE_CHUNKCOMPONENT_H
#define MINECRAFT_CLONE_CHUNKCOMPONENT_H
#include <glm/glm.hpp>
#include <memory>
#include <array>
#include "BlockType.h"
#include "components/Model.h"
#include "components/TempMesh.h"
#include "vector_extensions.h"

constexpr int CHUNK_SIZE = 32;
constexpr int CHUNK_ACTUAL_SIZE = CHUNK_SIZE + 2;
constexpr int MAX_Y = 256;
using BlockArray = std::array<BlockType, CHUNK_ACTUAL_SIZE * CHUNK_ACTUAL_SIZE * CHUNK_ACTUAL_SIZE>;

struct BlockVertex {
    int x, y, z;
    float u, v;
    int aoLvl;
};

// Block coords and Block Local coords
// Guards against passing the wrong coord type to a function
using BCoords = glm::ivec3;
using BLocalCoords = glm::ivec3;

struct Chunk {
    bool modified = true;
    Model model;
    TempMesh<BlockVertex> tempMesh;
    
    // storage order: y - x - z
    std::unique_ptr<BlockArray> blockTypes = std::make_unique<BlockArray>();;

    BlockType getBlock(const BLocalCoords &localCoords) const;
    void setBlock(const BLocalCoords &localCoords, BlockType blockType);
};

constexpr glm::ivec3 toChunk(const glm::vec3 &globalCoords) {
    return floor(globalCoords / static_cast<float>(CHUNK_SIZE));
}

constexpr BLocalCoords toLocal(const glm::vec3 &globalCoords) {
    const glm::ivec3 chunkCoords = toChunk(globalCoords);
    return floor(globalCoords) - (chunkCoords * CHUNK_SIZE) + glm::ivec3(1);
}

constexpr glm::ivec3 toGlobal(const glm::ivec3 &localCoords, const glm::ivec3 &chunkCoords) {
    return localCoords - glm::ivec3(1) + chunkCoords * CHUNK_SIZE;
}


#endif //MINECRAFT_CLONE_CHUNKCOMPONENT_H
