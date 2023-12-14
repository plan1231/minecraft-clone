//
// Created by Phil Lan on 2023-12-03.
//

#ifndef FACTORIES_H
#define FACTORIES_H
#include <entt/entt.hpp>
#include <glm/glm.hpp>

entt::entity makePlayer(entt::registry &registry);

entt::entity makeCrosshair(entt::registry &registry);

#endif //FACTORIES_H
