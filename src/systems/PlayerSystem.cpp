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
    auto [dist, loc] = raycast(transform.position + camera.posOffset, camera.front, true);
    if(dist > 0) {
        BlockType old = chunkManager.getBlock(loc);
        chunkManager.setBlock(loc, BlockType::DIRT);
    }
}

std::pair<float, glm::ivec3> PlayerSystem::raycast(const glm::vec3 &start, const glm::vec3 &dir, bool retLastAir) {
    const int m = 32;
    const int maxDist = 8;
    glm::ivec3 prevInt{};
    glm::vec3 curr = start;
    for(int step = 0; step < m * maxDist; step++) {
        glm::ivec3 currInt = floor(curr); // convert float to int. We must floor it or else it doesn't work for negative numbers

        if(currInt != prevInt || step == 0) {

            if(chunkManager.getBlock(currInt) != BlockType::AIR) {
                return {glm::distance(start, curr),  retLastAir ? prevInt : currInt};
            }
            prevInt = currInt;
        }
        curr += dir / static_cast<float>(m);
    }
    return {-1.0f, glm::ivec3{}};
}
