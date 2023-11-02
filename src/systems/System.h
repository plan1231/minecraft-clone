//
// Created by Phil Lan on 2023-10-27.
//

#ifndef MINECRAFT_CLONE_SYSTEM_H
#define MINECRAFT_CLONE_SYSTEM_H

#include <entt/entt.hpp>
#include "InputManager.h"

class System {
protected:
    entt::registry &registry;
    entt::dispatcher &dispatcher;
    InputManager &inputManager;
public:
    System(entt::registry &registry, entt::dispatcher &dispatcher, InputManager &inputManager);
    virtual void update(float dt) = 0;
};


#endif //MINECRAFT_CLONE_SYSTEM_H
