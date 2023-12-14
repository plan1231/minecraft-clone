//
// Created by Phil Lan on 2023-12-03.
//
#include "factories.h"
#include "Application.h"
#include "components/Transform.h"
#include "components/physics/geometry/AABB.h"
#include "components/physics/Dynamics.h"
#include "components/Camera.h"
#include "components/CharacterMovement.h"
#include "world/Chunk.h"
#include "components/Crosshair.h"
#include "components/Model.h"
#include "components/TempMesh.h"

entt::entity makePlayer(entt::registry &registry) {
    entt::entity player = registry.create();

    Transform &t = registry.emplace<Transform>(player, glm::vec3(0.0f, 120.0f, 0.0f),
                                glm::quat(),
                                glm::vec3(1.0f, 1.0f, 1.0f));

    registry.emplace<AABB>(player, AABB{
                               .min = t.position,
                               .max = t.position + glm::vec3{0.5f, 1.8f, 0.5f}
                           });

    registry.emplace<Dynamics>(player);

    registry.emplace<CharacterMovement>(player);

    Camera &c = registry.emplace<Camera>(player);
    c.posOffset = {0.25f, 1.8f, 0.25f};

    return player;
}

entt::entity makeCrosshair(entt::registry& registry) {
    entt::entity crosshair = registry.create();

    MeshPtr mesh = std::make_shared<Mesh>(VertexAttributes{
        VertexAttribute {2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0}
    }, GL_LINES);

    float x = WINDOW_WIDTH / 2.0f;
    float y = WINDOW_HEIGHT / 2.0f;

    float data[] = {
        x, y - CROSSHAIR_LENGTH,
        x, y + CROSSHAIR_LENGTH,

        x - CROSSHAIR_LENGTH, y,
        x + CROSSHAIR_LENGTH, y
        // 0.0, 0.0, 0.5, 0.5,
        // 0.0, 0.0, -0.3, 0.3

    };



    mesh->bufferData(data, sizeof(data), {0, 1, 2, 3});
    registry.emplace<Crosshair>(crosshair, Crosshair{
        .projectionMatrix = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, 0.0f, (float)1)
    });
    registry.emplace<Model>(crosshair, Model{.mesh =  mesh});

    return crosshair;
}