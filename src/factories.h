//
// Created by Phil Lan on 2023-12-03.
//

#ifndef FACTORIES_H
#define FACTORIES_H
#include <entt/entt.hpp>
#include <glm/glm.hpp>

entt::entity makePlayer(entt::registry &registry);

entt::entity makeChunk(entt::registry &registry, const glm::ivec3 &chunkCoords);

#endif //FACTORIES_H
