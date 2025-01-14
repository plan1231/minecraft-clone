//
// Created by Phil Lan on 2023-10-27.
//

#ifndef MINECRAFT_CLONE_GAME_H
#define MINECRAFT_CLONE_GAME_H
#include <vector>
#include <entt/entt.hpp>
#include <memory>
#include "managers/InputManager.h"

class System;

class Game {
    entt::registry registry;
    entt::dispatcher dispatcher;
    std::vector<System*> systems;
public:
    Game();
    void update(float dt);

};


#endif //MINECRAFT_CLONE_GAME_H
