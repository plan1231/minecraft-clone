//
// Created by Phil Lan on 2023-11-28.
//

#include "PhysicsSystem.h"

#include "collide.h"
#include "components/Transform.h"
#include "components/physics/Dynamics.h"
#include "components/physics/geometry/AABB.h"
#include <vector_extensions.h>
#include <stdio.h>
#include <vector>

PhysicsSystem::PhysicsSystem(entt::registry&registry, entt::dispatcher&dispatcher): System(registry, dispatcher) {
}

void PhysicsSystem::update(float dt) {
    registry.view<Dynamics, AABB, Transform>().each([&](Dynamics &dyn, AABB &aabb, Transform &transform) {
        glm::ivec3 minInt = floor(aabb.min);
        glm::ivec3 maxInt = floor(aabb.max);
        glm::vec3 oldPos = transform.position;
        for (int i = 0; i < 3; i++) {
            glm::vec3 adjVel = dyn.velocity * dt;
            std::vector<std::tuple<float, glm::vec3, glm::ivec3>> collisions;
            for (int y = minInt.y - 2; y <= maxInt.y + 2; y++) {
                for (int x = minInt.x - 4; x <= maxInt.x + 4; x++) {
                    for (int z = minInt.z - 4; z <= maxInt.z + 4; z++) {
                        if (chunkManager.getBlock({x, y, z}) == BlockType::AIR) continue;
                        AABB blkCollider{
                            .min = {x, y, z},
                            .max = {x + 1, y + 1, z + 1}
                        };

                        auto [collided, cTime, normal] = collide(blkCollider, aabb, adjVel);
                        if (!collided) continue;

                        collisions.emplace_back(cTime, normal, glm::ivec3{x, y, z});
                    }
                }
            }

            if (!collisions.empty()) {
                auto it = std::ranges::min_element(
                    collisions, [](const std::tuple<float, glm::vec3, glm::ivec3>&lhs,
                                   const std::tuple<float, glm::vec3, glm::ivec3>&rhs) {
                        return std::get<0>(lhs) < std::get<0>(rhs);
                    });
                auto [cTime, normal, loc] = *it;
                cTime -= 0.001;
                printf("got collision with block %d %d %d  normal %f %f %f, player loc %f %f %f\n", loc.x, loc.y, loc.z,
                       normal.x, normal.y, normal.z, transform.position.x, transform.position.y, transform.position.z);

                if (normal.x) {
                    dyn.velocity.x = 0;
                    transform.position.x += adjVel.x * cTime;
                }
                if (normal.y) {
                    dyn.velocity.y = 0;
                    transform.position.y += adjVel.y * cTime;
                }
                if (normal.z) {
                    dyn.velocity.z = 0;
                    transform.position.z += adjVel.z * cTime;
                }
            }
        }
        transform.position += dyn.velocity * dt;
        glm::vec3 dsplcmnt = transform.position - oldPos;
        aabb.max += dsplcmnt;
        aabb.min += dsplcmnt;
    });
}
