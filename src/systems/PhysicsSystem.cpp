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

#include "components/CharacterMovement.h"
#include "physics_constants.h"

PhysicsSystem::PhysicsSystem(entt::registry&registry, entt::dispatcher&dispatcher): System(registry, dispatcher) {
}

void PhysicsSystem::update(float dt) {
    registry.view<Dynamics, AABB, Transform, CharacterMovement>().each([&](Dynamics &dyn, AABB &aabb, Transform &transform, CharacterMovement &cm) {
        glm::ivec3 minInt = floor(aabb.min);
        glm::ivec3 maxInt = floor(aabb.max);
        glm::vec3 oldPos = transform.position;



        dyn.velocity += dyn.acceleration * dt;

        // apply gravity and clamp dropping speed
        dyn.velocity.y += g * dt;
        dyn.velocity.y = std::clamp(dyn.velocity.y, -TERM_SPEED_Y, TERM_SPEED_Y);

        // clamp xz plane movement speed
        glm::vec2 xzVel{dyn.velocity.x, dyn.velocity.z};
        float xzSpeed = glm::length(xzVel);

        if(xzSpeed > TERM_SPEED_XZ) {
            dyn.velocity.x *= (TERM_SPEED_XZ / xzSpeed);
            dyn.velocity.z *= (TERM_SPEED_XZ / xzSpeed);
        }


        // apply drag when not accelerating in the xz direction (i.e. player wants to stop)
        if(dyn.acceleration.x == 0.0f && dyn.acceleration.z == 0.0f && xzSpeed > 0.0f) {
            glm::vec2 xzNormalized = xzVel / xzSpeed;
            dyn.velocity.x -= dyn.velocity.x > 0 ?  std::min(xzNormalized.x * DRAG_DEACCELERATION * dt, dyn.velocity.x) : std::max(xzNormalized.x * DRAG_DEACCELERATION * dt, dyn.velocity.x);
            dyn.velocity.z -= dyn.velocity.z > 0 ?  std::min(xzNormalized.y * DRAG_DEACCELERATION * dt, dyn.velocity.z) : std::max(xzNormalized.y * DRAG_DEACCELERATION * dt, dyn.velocity.z);
        }


        int speedX = ceil(abs(dyn.velocity.x));
        int speedY = ceil(abs(dyn.velocity.y));
        int speedZ = ceil(abs(dyn.velocity.z));

        for (int i = 0; i < 3; i++) {
            glm::vec3 adjVel = dyn.velocity * dt;
            std::vector<std::tuple<float, glm::vec3, glm::ivec3>> collisions;
            for (int y = minInt.y - 4 - speedY; y <= maxInt.y + 4 + speedY; y++) {
                for (int x = minInt.x - 4 - speedX; x <= maxInt.x + 4 + speedX; x++) {
                    for (int z = minInt.z - 4 - speedZ; z <= maxInt.z + 4 + speedZ; z++) {
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
                printf("got collision with block %d %d %d  normal %f %f %f, player loc %f %f %f\n", loc.x, loc.y, loc.z,
                       normal.x, normal.y, normal.z, transform.position.x, transform.position.y, transform.position.z);

                if (normal.x) {
                    dyn.velocity.x = 0;
                    transform.position.x += adjVel.x * cTime;
                }
                if (normal.y) {
                    dyn.velocity.y = 0;
                    transform.position.y += adjVel.y * cTime;
                    cm.isGrounded = true;
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
