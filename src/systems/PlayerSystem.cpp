//
// Created by Phil Lan on 2023-10-30.
//

#include "PlayerSystem.h"
#include "events/CameraUpdateEvent.h"
#include "components/Transform.h"
#include "components/Camera.h"
#include "world/Chunk.h"
#include "components/physics/Dynamics.h"
#include <iostream>
#include "vector_extensions.h"
#include <entt/entt.hpp>
#include "GameEntities.h"
#include "components/CharacterMovement.h"
#include "physics_constants.h"

PlayerSystem::PlayerSystem(entt::registry &registry, entt::dispatcher &dispatcher) :
        System(registry, dispatcher) {
    playerEntity = entt::locator<GameEntities>::value().player;
}

void PlayerSystem::update(float dt) {
    Camera &camera = registry.get<Camera>(playerEntity);
    Transform &transform = registry.get<Transform>(playerEntity);
    CharacterMovement &cm = registry.get<CharacterMovement>(playerEntity);
    updateRotPos(dt, camera, transform, cm);

    static float timeSinceLastAction = 1.0f;
    timeSinceLastAction += dt;
    if(timeSinceLastAction < 0.25f) return;

    if (inputManager.getMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        placeBlock(camera, transform);
        timeSinceLastAction = 0.0f;
    }
    else if(inputManager.getMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        removeBlock(camera, transform);
        timeSinceLastAction = 0.0f;
    }
}

void PlayerSystem::updateRotPos(float dt, Camera &camera, Transform &transform, CharacterMovement &cm) {

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

    glm::vec3 xzAcc{}; // acceleration in the xz directions
    if (inputManager.getKey(GLFW_KEY_W) == GLFW_PRESS)
        xzAcc += forward;
    if (inputManager.getKey(GLFW_KEY_S) == GLFW_PRESS)
        xzAcc -= forward;
    if (inputManager.getKey(GLFW_KEY_D) == GLFW_PRESS)
        xzAcc += right;
    if (inputManager.getKey(GLFW_KEY_A) == GLFW_PRESS)
        xzAcc -= right;
    if (inputManager.getKey(GLFW_KEY_SPACE) == GLFW_PRESS && cm.isGrounded) {
        dynamics.velocity.y = sqrt(2.0f * JUMP_HEIGHT * -g);
        cm.isGrounded = false;
    }
    if (inputManager.getKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        xzAcc -= up;
    lastCursorPos = cursorPos;
    if(xzAcc != glm::vec3(0.0f)) {
        xzAcc = glm::normalize(xzAcc) * MOVEMENT_ACCELERATION;
    }
    dynamics.acceleration = xzAcc;
}

void PlayerSystem::placeBlock(Camera &camera, Transform &transform) {
    auto [dist, loc] = raycast(transform.position + camera.posOffset, camera.front, true);
    if(dist > 0) {
        chunkManager.setBlock(loc, BlockType::DIRT);
    }
}

void PlayerSystem::removeBlock(Camera& camera, Transform& transform) {
    auto [dist, loc] = raycast(transform.position + camera.posOffset, camera.front, false);
    if(dist > 0) {
        chunkManager.setBlock(loc, BlockType::AIR);
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
