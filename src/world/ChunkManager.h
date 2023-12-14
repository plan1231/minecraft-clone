//
// Created by Phil Lan on 2023-11-03.
//

#ifndef MINECRAFT_CLONE_CHUNKMANAGER_H
#define MINECRAFT_CLONE_CHUNKMANAGER_H
#include <unordered_map>
#include "BlockType.h"
#include "world/Chunk.h"
#include "rendering/Texture.h"
#include "PerlinNoise.h"

namespace std {
    template<>
    struct hash<glm::ivec3> {
        size_t operator()(const glm::ivec3 &vec) const {
            size_t h1 = std::hash<int>()(vec.x);
            size_t h2 = std::hash<int>()(vec.y);
            size_t h3 = std::hash<int>()(vec.z);

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}

struct AdjacentChunks {
    Chunk *nextX, *prevX, *nextY, *prevY, *nextZ, *prevZ;
};

class ChunkManager {
public:
    std::unordered_map<glm::ivec3, Chunk> chunks;

    BlockType getBlock(const glm::ivec3 &coords);

    void setBlock(const glm::ivec3 &coords, BlockType blockType);

    Chunk& loadChunk(const glm::ivec3 &chunkCoords);

    ChunkManager();

    AdjacentChunks getAdjacent(const glm::ivec3 &chunkCoords);
private:
    PerlinNoise perlinNoise;
    Chunk *getChunk(const glm::ivec3 &chunkCoords);
};


#endif //MINECRAFT_CLONE_CHUNKMANAGER_H
