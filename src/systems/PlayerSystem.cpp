//
// Created by Phil Lan on 2023-10-30.
//

#include "PlayerSystem.h"
#include "events/CameraUpdateEvent.h"
#include "components/TransformComponent.h"
#include "components/CameraComponent.h"
#include "components/ChunkComponent.h"
#include <iostream>
#include "vector_extensions.h"
PlayerSystem::PlayerSystem(entt::registry &registry, entt::dispatcher &dispatcher) :
        System(registry, dispatcher) {
    playerEntity = registry.create();
    TransformComponent &t = registry.emplace<TransformComponent>(playerEntity, glm::vec3(0.0f, 105.0f, 0.0f),
                                                                 glm::quat(),
                                                                 glm::vec3(1.0f, 1.0f, 1.0f));
    registry.emplace<CameraComponent>(playerEntity).calculateMatrices(t);
}

void PlayerSystem::update(float dt) {
    CameraComponent &camera = registry.get<CameraComponent>(playerEntity);
    TransformComponent &transform = registry.get<TransformComponent>(playerEntity);
    updateRotPos(dt, camera, transform);
    if (inputManager.getMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        placeBlock(camera, transform);
    }
}

void PlayerSystem::updateRotPos(float dt, CameraComponent &camera, TransformComponent &transform) {

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
    glm::vec3 velocity{};
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

    bool positionChanged = false;
    if (velocity != glm::vec3{}) {
        transform.position += glm::normalize(velocity) * dt * 5.0f;
        positionChanged = true;
    }

    if (rotationChanged || positionChanged) {
        camera.calculateMatrices(transform);
        dispatcher.trigger<CameraUpdateEvent>(CameraUpdateEvent{playerEntity});
    }
    lastCursorPos = cursorPos;
}

void PlayerSystem::placeBlock(CameraComponent &camera, TransformComponent &transform) {
    glm::ivec2 pChunkCoord = toChunk(transform.position);
    float bestDist = -1;
    glm::ivec3 bestRes{};
    for (auto &it: chunkManager.chunks) {
        if(manhattan(it.first, pChunkCoord) > 2) continue;
        auto [dist, loc] = chunkRayHit(it.first, registry.get<ChunkComponent>(it.second), transform.position, camera.front);
        if(dist > 0 && (bestDist < 0 || dist < bestDist)) {
            bestDist = dist;
            bestRes = loc;
        }
    }
    if(bestDist > 0) {
        chunkManager.setBlock(bestRes, BlockType::DIRT);
    }
}

std::pair<float, glm::ivec3> PlayerSystem::chunkRayHit(const glm::ivec2 &chunkCoords, ChunkComponent &chunk, const glm::vec3 &start, const glm::vec3 &dir) {
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
