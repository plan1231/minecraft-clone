//
// Created by Phil Lan on 2023-10-27.
//

#ifndef MINECRAFT_CLONE_SYSTEM_H
#define MINECRAFT_CLONE_SYSTEM_H

#include <entt/entt.hpp>

#include "threading/ThreadPool.h"
#include "managers/InputManager.h"
#include "world/ChunkManager.h"

class System {
protected:
    entt::registry &registry;
    entt::dispatcher &dispatcher;
    InputManager &inputManager;
    ChunkManager &chunkManager;
    ThreadPool &threadPool;
public:
    System(entt::registry &registry, entt::dispatcher &dispatcher);
    virtual void update(float dt) = 0;
    virtual ~System();
};


#endif //MINECRAFT_CLONE_SYSTEM_H
