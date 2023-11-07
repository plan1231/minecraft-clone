//
// Created by Phil Lan on 2023-10-30.
//

#ifndef MINECRAFT_CLONE_PLAYERSYSTEM_H
#define MINECRAFT_CLONE_PLAYERSYSTEM_H
#include "System.h"
#include <entt/entt.hpp>

class PlayerSystem: public System {
    entt::entity playerEntity;

public:
    PlayerSystem(entt::registry& registry, entt::dispatcher& dispatcher, InputManager& inputManager);
    void update(float dt) override;
};


#endif //MINECRAFT_CLONE_PLAYERSYSTEM_H
