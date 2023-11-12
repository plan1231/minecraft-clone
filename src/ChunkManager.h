//
// Created by Phil Lan on 2023-11-03.
//

#ifndef MINECRAFT_CLONE_CHUNKMANAGER_H
#define MINECRAFT_CLONE_CHUNKMANAGER_H
#include <unordered_map>
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "BlockType.h"
#include "components/ChunkComponent.h"

namespace std {
    template<>
    struct hash<glm::ivec3> {
        size_t operator()(const glm::ivec3&vec) const {
            size_t h1 = std::hash<int>()(vec.x);
            size_t h2 = std::hash<int>()(vec.y);
            size_t h3 = std::hash<int>()(vec.z);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}

class ChunkManager {
public:
    std::unordered_map<glm::ivec3, entt::entity> chunks;

    BlockType getBlock(const glm::ivec3 &coords);

    void setBlock(const glm::ivec3 &coords, BlockType blockType);

    void loadChunk(const glm::ivec3 &chunkCoords, BlockType b);
    ChunkManager(entt::registry &registry);

    struct ChunkLocalCoords {
        glm::ivec3 chunkCoords;
        glm::ivec3 localCoords;
    };

private:
    entt::registry &registry;
};


#endif //MINECRAFT_CLONE_CHUNKMANAGER_H
