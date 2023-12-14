//
// Created by Phil Lan on 2023-12-14.
//

#pragma once
#include "System.h"
#include "TerrainGenerator.h"
#include "components/TempMesh.h"
#include "world/Chunk.h"
constexpr int LOAD_DISTANCE = 5;

class ChunkSystem: public System {
public:
    void update(float dt) override;
    ChunkSystem(entt::registry &registry, entt::dispatcher &dispatcher);
private:
    TerrainGenerator terrainGen;


    static std::array<int, 4> getFaceAo(const Chunk &chunk, const BCoords &coords, Face face);
    static void emitFace(TempMesh<BlockVertex> &tmpMesh, Chunk &chunk, const glm::ivec3 &localCoords, BlockType blockType, Face face);
    static void emitBlock(TempMesh<BlockVertex> &tmpMesh, Chunk &chunk, const glm::ivec3 &localCoords);
};
