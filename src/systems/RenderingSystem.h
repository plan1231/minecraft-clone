//
// Created by Phil Lan on 2023-10-27.
//

#ifndef MINECRAFT_CLONE_RENDERINGSYSTEM_H
#define MINECRAFT_CLONE_RENDERINGSYSTEM_H

#include "System.h"
#include "events/CameraUpdateEvent.h"
#include "rendering/Shader.h"

class RenderingSystem : public System {
    ShaderPtr shader;

    void onCameraUpdate(CameraUpdateEvent &event);

public:
    RenderingSystem(entt::registry &registry, entt::dispatcher &dispatch);

    void update(float dt) override;
};


#endif //MINECRAFT_CLONE_RENDERINGSYSTEM_H
