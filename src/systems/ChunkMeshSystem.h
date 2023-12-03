//
// Created by Phil Lan on 2023-11-05.
//

#ifndef MINECRAFT_CLONE_CHUNKMESHSYSTEM_H
#define MINECRAFT_CLONE_CHUNKMESHSYSTEM_H

#include "System.h"
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include "BlockType.h"
#include "components/Chunk.h"
class ChunkMeshSystem : public System {
public:
    void update(float dt) override;
    ChunkMeshSystem(entt::registry &registry, entt::dispatcher &dispatcher);
private:
    struct Vertex {
        int x, y, z;
        float u, v;
        int aoLvl;
    };

    static std::array<int, 4> getFaceAo(const Chunk &chunk, const BCoords &coords, Face face);
    static void emitFace(std::vector<Vertex> &vertices, std::vector<uint> &indices, const Chunk &chunk, const glm::ivec3 &localCoords, BlockType blockType, Face face);

    static void emitBlock(std::vector<Vertex> &vertices, std::vector<uint> &indices, const Chunk &chunk, const glm::ivec3 &localCoords);
};


#endif //MINECRAFT_CLONE_CHUNKMESHSYSTEM_H
