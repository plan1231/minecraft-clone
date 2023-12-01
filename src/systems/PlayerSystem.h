//
// Created by Phil Lan on 2023-10-30.
//

#ifndef MINECRAFT_CLONE_PLAYERSYSTEM_H
#define MINECRAFT_CLONE_PLAYERSYSTEM_H

#include "System.h"
#include <entt/entt.hpp>
#include "components/Camera.h"
#include "components/Transform.h"
#include "ChunkManager.h"

class PlayerSystem : public System {
public:
    PlayerSystem(entt::registry&registry, entt::dispatcher&dispatcher);

    void update(float dt) override;

private:
    entt::entity playerEntity;

    void updateRotPos(float dt, Camera &camera, Transform &transform);

    void placeBlock(Camera &camera, Transform &transform);

    std::pair<float, glm::ivec3> chunkRayHit(const glm::ivec2 &chunkCoords, Chunk &chunk, const glm::vec3&start, const glm::vec3 &dir);
};


#endif //MINECRAFT_CLONE_PLAYERSYSTEM_H
