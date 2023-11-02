//
// Created by Phil Lan on 2023-10-30.
//

#ifndef MINECRAFT_CLONE_CAMERASYSTEM_H
#define MINECRAFT_CLONE_CAMERASYSTEM_H
#include "System.h"
#include <entt/entt.hpp>

class CameraSystem: public System {
    entt::entity cameraEntity;

public:
    CameraSystem(entt::registry& registry, entt::dispatcher& dispatcher, InputManager& inputManager);
    void update(float dt) override;
};


#endif //MINECRAFT_CLONE_CAMERASYSTEM_H
