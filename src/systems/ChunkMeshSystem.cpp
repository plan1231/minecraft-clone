//
// Created by Phil Lan on 2023-11-05.
//

#include "ChunkMeshSystem.h"

#include "components/CameraComponent.h"
#include "components/ChunkComponent.h"
#include "components/MeshComponent.h"
#include "components/TransformComponent.h"
#include "systems/ChunkLoadingSystem.h"
// Credit: https://github.com/jdah/minecraft-again/blob/master/src/level/chunk_renderer.cpp
/*  3D CUBE
 *  1-------2
 *  |5------+6
 *  ||      ||
 *  ||      ||
 *  0+------3|
 *   4-------7
 *
 *  bottom:
 *  4-------7
 *  |       |
 *  |       |
 *  |       |
 *  0-------3
 *
 * top:
 *  5-------6
 *  |       |
 *  |       |
 *  |       |
 *  1-------2
 *
 * vertices:
 *  (0, 0, 0)
 *  (0, 1, 0)
 *  (1, 1, 0)
 *  (1, 0, 0)
 *  (0, 0, 1)
 *  (0, 1, 1)
 *  (1, 1, 1)
 *  (1, 0, 1)
 *
 * indices:
 * 4, 7, 6, 4, 6, 5, // (south (+z))
 * 3, 0, 1, 3, 1, 2, // (north (-z))
 * 7, 3, 2, 7, 2, 6, // (east  (+x))
 * 0, 4, 5, 0, 5, 1, // (west  (-x))
 * 2, 1, 5, 2, 5, 6, // (up    (+y))
 * 0, 3, 7, 0, 7, 4  // (down  (-y))
 */

// indices, within each list of 6 cube indices, which represent are the 4
// unique vertices which make up each face
static const uint UNIQUE_INDICES[] = {0, 1, 2, 5};

// indices into emitted vertices which make up the two faces for a cube face
static const uint FACE_INDICES[] = {0, 1, 2, 0, 2, 3};

static const uint CUBE_INDICES[] = {
    4, 7, 6, 4, 6, 5, // (south (+z))
    3, 0, 1, 3, 1, 2, // (north (-z))
    7, 3, 2, 7, 2, 6, // (east  (+x))
    0, 4, 5, 0, 5, 1, // (west  (-x))
    2, 1, 5, 2, 5, 6, // (up    (+y))
    0, 3, 7, 0, 7, 4 // (down  (-y))
};

static const glm::ivec3 CUBE_VERTICES[] = {
    glm::ivec3(0, 0, 0),
    glm::ivec3(0, 1, 0),
    glm::ivec3(1, 1, 0),
    glm::ivec3(1, 0, 0),
    glm::ivec3(0, 0, 1),
    glm::ivec3(0, 1, 1),
    glm::ivec3(1, 1, 1),
    glm::ivec3(1, 0, 1)
};

static const glm::vec3 CUBE_NORMALS[] = {
    glm::vec3(0, 0, 1),
    glm::vec3(0, 0, -1),
    glm::vec3(1, 0, 0),
    glm::vec3(-1, 0, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(0, -1, 0),
};

static const glm::vec2 CUBE_UVS[] = {
    glm::vec2(0, 0),
    glm::vec2(0.0625, 0),
    glm::vec2(0.0625, 0.0625),
    glm::vec2(0, 0.0625),
};


void ChunkMeshSystem::update(float dt) {
    auto playerView = registry.view<TransformComponent, CameraComponent>();
    playerView.each([&](TransformComponent&playerTransform, auto) {
        glm::ivec2 pChunkCoords = toChunk(playerTransform.position);
        registry.view<ChunkComponent, MeshComponent, TransformComponent>().each(
            [&](ChunkComponent&chunk, MeshComponent&mesh, TransformComponent&transform) {
                if (!chunk.modified) return;
                glm::ivec2 chunkCoords = toChunk(transform.position);
                if (abs(chunkCoords.x - pChunkCoords.x) > LOAD_DISTANCE - 1 || abs(chunkCoords.y - pChunkCoords.y) >
                    LOAD_DISTANCE - 1)
                    return;
                std::vector<Vertex> vertices;
                std::vector<uint> indices;
                AdjacentChunks adjChunks = chunkManager.getAdjacent(chunkCoords);
                for (uint y = 0; y < CHUNK_HEIGHT; y++) {
                    for (uint x = 0; x < CHUNK_LENGTH; x++) {
                        for (int z = 0; z < CHUNK_LENGTH; z++) {
                            BlockType blockType = chunk.getBlock({x, y, z});
                            if (blockType == BlockType::AIR) continue;
                            emitBlock(vertices, indices, chunk, adjChunks, {x, y, z});
                        }
                    }
                }
                mesh.mesh->bufferData(reinterpret_cast<const char *>(vertices.data()), vertices.size() * sizeof(Vertex),
                                      indices, GL_STATIC_DRAW);
                chunk.modified = false;
            });
    });
}

void ChunkMeshSystem::emitBlock(
    std::vector<Vertex> &vertices,
    std::vector<uint> &indices,
    const ChunkComponent &chunk,
    const AdjacentChunks &adjChunks,
    const glm::ivec3 &localCoords) {
    BlockType currBlockType = chunk.getBlock(localCoords);

    // +z face
    if (localCoords.z == CHUNK_LENGTH - 1) {
        if (adjChunks.nextZ != nullptr && BlockTypeInfo::get(
                adjChunks.nextZ->getBlock({localCoords.x, localCoords.y, 0})).isTransparent) {
            emitFace(vertices, indices, localCoords, currBlockType, 0);
        }
    }
    else if (BlockTypeInfo::get(chunk.getBlock({localCoords.x, localCoords.y, localCoords.z + 1})).isTransparent) {
        emitFace(vertices, indices, localCoords, currBlockType, 0);
    }

    // -z face
    if (localCoords.z == 0) {
        if (adjChunks.prevZ != nullptr && BlockTypeInfo::get(adjChunks.prevZ->getBlock({
                localCoords.x, localCoords.y, CHUNK_LENGTH - 1
            })).isTransparent) {
            emitFace(vertices, indices, localCoords, currBlockType, 1);
        }
    }
    else if (BlockTypeInfo::get(chunk.getBlock({localCoords.x, localCoords.y, localCoords.z - 1})).isTransparent) {
        emitFace(vertices, indices, localCoords, currBlockType, 1);
    }

    // +x face
    if (localCoords.x == CHUNK_LENGTH - 1) {
        if (adjChunks.nextX != nullptr && BlockTypeInfo::get(
                adjChunks.nextX->getBlock({0, localCoords.y, localCoords.z})).isTransparent) {
            emitFace(vertices, indices, localCoords, currBlockType, 2);
        }
    }
    else if (BlockTypeInfo::get(chunk.getBlock({localCoords.x + 1, localCoords.y, localCoords.z})).isTransparent) {
        emitFace(vertices, indices, localCoords, currBlockType, 2);
    }

    // -x face
    if (localCoords.x == 0) {
        if (adjChunks.prevX != nullptr && BlockTypeInfo::get(adjChunks.prevX->getBlock({
                CHUNK_LENGTH - 1, localCoords.y, localCoords.z
            })).isTransparent) {
            emitFace(vertices, indices, localCoords, currBlockType, 3);
        }
    }
    else if (BlockTypeInfo::get(chunk.getBlock({localCoords.x - 1, localCoords.y, localCoords.z})).isTransparent) {
        emitFace(vertices, indices, localCoords, currBlockType, 3);
    }

    // +y face
    if (localCoords.y == CHUNK_HEIGHT - 1 || BlockTypeInfo::get(chunk.getBlock({
            localCoords.x, localCoords.y + 1, localCoords.z
        })).isTransparent) {
        emitFace(vertices, indices, localCoords, currBlockType, 4);
    }

    // -y face
    if (localCoords.y != 0 && BlockTypeInfo::get(chunk.getBlock({localCoords.x, localCoords.y - 1, localCoords.z})).
        isTransparent) {
        emitFace(vertices, indices, localCoords, currBlockType, 5);
    }
}

void ChunkMeshSystem::emitFace(std::vector<Vertex>&vertices, std::vector<uint> &indices, const glm::ivec3 &localCoords,
                               BlockType blockType,
                               uint face) {
    uint offset = vertices.size();
    for (uint corner = 0; corner < 4; corner++) {
        // 4 vertices per face
        const glm::ivec3&cubeVertex = CUBE_VERTICES[CUBE_INDICES[6 * face + UNIQUE_INDICES[corner]]];
        const glm::vec2&uv = BlockTypeInfo::get(blockType).textCoords(face, corner);
        const Vertex v{
            .x = localCoords.x + cubeVertex.x,
            .y = localCoords.y + cubeVertex.y,
            .z = localCoords.z + cubeVertex.z,
            .u = uv.x,
            .v = uv.y,
        };
        vertices.push_back(v);
    }
    indices.insert(indices.end(), {offset, offset + 1, offset + 2, offset, offset + 2, offset + 3});
}

ChunkMeshSystem::ChunkMeshSystem(entt::registry &registry, entt::dispatcher &dispatcher)
    : System(registry, dispatcher) {
}
