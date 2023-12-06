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
#include "components/TempMesh.h"

class ChunkMeshSystem : public System {
public:
    void update(float dt) override;
    ChunkMeshSystem(entt::registry &registry, entt::dispatcher &dispatcher);
private:


    static std::array<int, 4> getFaceAo(const Chunk &chunk, const BCoords &coords, Face face);
    static void emitFace(TempMesh<BlockVertex> &tmpMesh, Chunk &chunk, const glm::ivec3 &localCoords, BlockType blockType, Face face);
    static void emitBlock(TempMesh<BlockVertex> &tmpMesh, Chunk &chunk, const glm::ivec3 &localCoords);
};


#endif //MINECRAFT_CLONE_CHUNKMESHSYSTEM_H
