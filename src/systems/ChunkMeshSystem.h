//
// Created by Phil Lan on 2023-11-05.
//

#ifndef MINECRAFT_CLONE_CHUNKMESHSYSTEM_H
#define MINECRAFT_CLONE_CHUNKMESHSYSTEM_H

#include "System.h"
#include <vector>
#include <glm/glm.hpp>

class ChunkMeshSystem : public System {
public:
    void update(float dt) override;
    ChunkMeshSystem(entt::registry &registry, entt::dispatcher &dispatcher);
private:
    struct Vertex {
        int x, y, z;
        float u, v;
    };


    static void emitFace(std::vector<Vertex> &vertices, std::vector<uint> &indices, const glm::ivec3 &localCoords, uint face);

    static void emitBlock(std::vector<Vertex> &vertices, std::vector<uint> &indices, const glm::ivec3 &localCoords);
};


#endif //MINECRAFT_CLONE_CHUNKMESHSYSTEM_H
