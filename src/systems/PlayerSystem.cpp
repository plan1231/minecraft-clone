//
// Created by Phil Lan on 2023-10-30.
//

#include "PlayerSystem.h"
#include "events/CameraUpdateEvent.h"
#include "components/Transform.h"
#include "components/Camera.h"
#include "components/Chunk.h"
#include "components/physics/Dynamics.h"
#include <iostream>
#include "vector_extensions.h"
#include <entt/entt.hpp>
#include "GameEntities.h"

PlayerSystem::PlayerSystem(entt::registry &registry, entt::dispatcher &dispatcher) :
        System(registry, dispatcher) {
    playerEntity = entt::locator<GameEntities>::value().player;
    Transform &t = registry.emplace<Transform>(playerEntity, glm::vec3(0.0f, 120.0f, 0.0f),
                                                                 glm::quat(),
                                                                 glm::vec3(1.0f, 1.0f, 1.0f));
    registry.emplace<AABB>(playerEntity, AABB{
        .min = t.position,
        .max = t.position + glm::vec3{0.5f, 0.5f, 0.5f}
    });
    registry.emplace<Dynamics>(playerEntity);
    Camera &c = registry.emplace<Camera>(playerEntity);
    c.posOffset = {0.25f, 0.25f, 0.25f};
}

void PlayerSystem::update(float dt) {
    Camera &camera = registry.get<Camera>(playerEntity);
    Transform &transform = registry.get<Transform>(playerEntity);
    updateRotPos(dt, camera, transform);

    static float timeSinceLastAction = 1.0f;
    timeSinceLastAction += dt;
    if(timeSinceLastAction < 0.5f) return;

    if (inputManager.getMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        placeBlock(camera, transform);
        timeSinceLastAction = 0.0f;
    }
    else if(inputManager.getMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        timeSinceLastAction = 0.0f;
    }
}

void PlayerSystem::updateRotPos(float dt, Camera &camera, Transform &transform) {

    static glm::dvec2 lastCursorPos = inputManager.getCursorPos();
    glm::dvec2 cursorPos = inputManager.getCursorPos();
    glm::dvec2 cursorChange = cursorPos - lastCursorPos;

    bool rotationChanged = false;
    if (cursorChange != glm::dvec2()) {
        rotationChanged = true;
        camera.yaw += cursorChange.x * 0.05;
        camera.pitch -= cursorChange.y * 0.05;

        camera.pitch = glm::clamp(camera.pitch, -89.0f, 89.0f);

        if (camera.yaw >= 360.0f) camera.yaw -= 360.0f;
        else if (camera.yaw <= -360.0f) camera.yaw -= 360.0f;
    }

    const glm::vec3 forward{camera.front.x, 0.0, camera.front.z};
    const glm::vec3 right{camera.right.x, 0.0, camera.right.z};
    const glm::vec3 up{0.0, 1.0, 0.0};
    Dynamics &dynamics = registry.get<Dynamics>(playerEntity);
    glm::vec3 &velocity = dynamics.velocity;
    velocity = glm::vec3(0.0f);
    if (inputManager.getKey(GLFW_KEY_W) == GLFW_PRESS)
        velocity += forward;
    if (inputManager.getKey(GLFW_KEY_S) == GLFW_PRESS)
        velocity -= forward;
    if (inputManager.getKey(GLFW_KEY_D) == GLFW_PRESS)
        velocity += right;
    if (inputManager.getKey(GLFW_KEY_A) == GLFW_PRESS)
        velocity -= right;
    if (inputManager.getKey(GLFW_KEY_SPACE) == GLFW_PRESS)
        velocity += up;
    if (inputManager.getKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        velocity -= up;
    lastCursorPos = cursorPos;
    if(velocity != glm::vec3(0.0f)) {
        velocity = glm::normalize(velocity) * 4.17f;
    }
}

void PlayerSystem::placeBlock(Camera &camera, Transform &transform) {
    glm::ivec2 pChunkCoord = toChunk(transform.position);
    float bestDist = -1;
    glm::ivec3 bestRes{};
    for (auto &it: chunkManager.chunks) {
        if(manhattan(it.first, pChunkCoord) > 2) continue;
        auto [dist, loc] = chunkRayHit(it.first, registry.get<Chunk>(it.second), transform.position + camera.posOffset, camera.front);
        if(dist > 0 && (bestDist < 0 || dist < bestDist)) {
            bestDist = dist;
            bestRes = loc;
        }
    }
    if(bestDist > 0) {
        BlockType old = chunkManager.getBlock(bestRes);
        printf("Placing block at %d %d %d, was originally %d\n",bestRes.x, bestRes.y, bestRes.z, old);
        chunkManager.setBlock(bestRes, BlockType::DIRT);
    }
}

std::pair<float, glm::ivec3> PlayerSystem::chunkRayHit(const glm::ivec2 &chunkCoords, Chunk &chunk, const glm::vec3 &start, const glm::vec3 &dir) {
    const int m = 32;
    const int maxDist = 8;
    glm::ivec3 prevPos{};
    glm::vec3 curr = start;
    for(int step = 0; step < m * maxDist; step++) {
        glm::ivec3 currInt = floor(curr); // convert float to int

        if(currInt != prevPos || step == 0) {
            glm::ivec2 currChunkCoords = toChunk(curr);
            glm::ivec3 localCoords = toLocal(currInt);

            if(currChunkCoords == chunkCoords && chunk.getBlock(localCoords) != BlockType::AIR) {
                return {glm::distance(start, curr), prevPos};
            }
            prevPos = currInt;
        }
        curr += dir / static_cast<float>(m);
    }
    return {-1.0f, glm::ivec3{}};
}
