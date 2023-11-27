//
// Created by Phil Lan on 2023-11-05.
//

#ifndef MINECRAFT_CLONE_CHUNKLOADINGSYSTEM_H
#define MINECRAFT_CLONE_CHUNKLOADINGSYSTEM_H

#include "System.h"
#include "TerrainGenerator.h"
#define LOAD_DISTANCE 5

class ChunkLoadingSystem: public System {
public:
    ChunkLoadingSystem(entt::registry &registry, entt::dispatcher &dispatcher);
    void update(float dt) override;
private:
    TerrainGenerator terrainGen;
};


#endif //MINECRAFT_CLONE_CHUNKLOADINGSYSTEM_H
