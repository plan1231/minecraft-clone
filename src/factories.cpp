//
// Created by Phil Lan on 2023-12-03.
//
#include "factories.h"
#include "components/Transform.h"
#include "components/physics/geometry/AABB.h"
#include "components/physics/Dynamics.h"
#include "components/Camera.h"
#include "components/Chunk.h"
#include "components/Model.h"

entt::entity makePlayer(entt::registry &registry) {
    entt::entity player = registry.create();

    Transform &t = registry.emplace<Transform>(player, glm::vec3(0.0f, 120.0f, 0.0f),
                                glm::quat(),
                                glm::vec3(1.0f, 1.0f, 1.0f));

    registry.emplace<AABB>(player, AABB{
                               .min = t.position,
                               .max = t.position + glm::vec3{0.5f, 0.5f, 0.5f}
                           });

    registry.emplace<Dynamics>(player);

    Camera &c = registry.emplace<Camera>(player);
    c.posOffset = {0.25f, 0.25f, 0.25f};

    return player;
}


entt::entity makeChunk(entt::registry& registry, const glm::ivec3& chunkCoords) {
    // todo: refactor resource loading
    static TexturePtr chunkTexture = std::make_shared<Texture>(ASSETS_PATH"/textures/blocks.png");
    entt::entity chunk = registry.create();

    MeshPtr meshPtr = std::make_shared<Mesh>(VertexAttributes{
        VertexAttribute{3, GL_FLOAT, GL_FALSE, 5 * sizeof(float) + 1 * sizeof(unsigned int), 0},
        VertexAttribute{
            2, GL_FLOAT, GL_FALSE, 5 * sizeof(float) + 1 * sizeof(unsigned int),
            (void *)(3 * sizeof(float))
        },
        VertexAttribute{1, GL_FLOAT, GL_FALSE, 5 * sizeof(float) + 1 * sizeof(unsigned int),
            (void *)(5 * sizeof(float))
        }
    }, GL_TRIANGLES);

    registry.emplace<Chunk>(chunk, Chunk(BlockType::AIR));
    registry.emplace<Model>(chunk, Model{ meshPtr, chunkTexture});
    registry.emplace<Transform>(chunk, glm::vec3{chunkCoords.x * CHUNK_SIZE, chunkCoords.y * CHUNK_SIZE, chunkCoords.z * CHUNK_SIZE},
                                glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
    return chunk;
}