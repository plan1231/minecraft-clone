//
// Created by Phil Lan on 2023-11-05.
//

#include "ChunkMeshSystem.h"
#include "components/ChunkComponent.h"
#include "components/MeshComponent.h"
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
        0, 3, 7, 0, 7, 4  // (down  (-y))
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
        glm::vec3( 0,  0,  1),
        glm::vec3( 0,  0, -1),
        glm::vec3( 1,  0,  0),
        glm::vec3(-1,  0,  0),
        glm::vec3( 0,  1,  0),
        glm::vec3( 0, -1,  0),
};

static const glm::vec2 CUBE_UVS[] = {
        glm::vec2(0, 0),
        glm::vec2(1, 0),
        glm::vec2(1, 1),
        glm::vec2(0, 1),
};



void ChunkMeshSystem::update(float dt) {
    registry.view<ChunkComponent, MeshComponent>().each([&](ChunkComponent &chunk, MeshComponent &mesh) {
        if(!chunk.modified) return;
        std::vector<Vertex> vertices;
        std::vector<uint> indices;
        for (uint x = 0; x < CHUNK_LENGTH; x++) {
            for (uint y = 0; y < CHUNK_LENGTH; y++) {
                for (int z = 0; z < CHUNK_LENGTH; z++) {
                    if((*chunk.blockTypes)[x * CHUNK_LENGTH * CHUNK_LENGTH + y * CHUNK_LENGTH + z] == BlockType::AIR) continue;
                    emitBlock(vertices, indices, {x, y, z});
                }
            }
        }
        mesh.geometry->bufferData(reinterpret_cast<const char *>(vertices.data()), vertices.size() * sizeof(Vertex),
                                  indices, GL_STATIC_DRAW);
        chunk.modified = false;
    });
}

void ChunkMeshSystem::emitBlock(
        std::vector<Vertex> &vertices,
        std::vector<uint> &indices,
        const glm::ivec3 &localCoords) {
    for (uint face = 0; face < 6; face++) { // 6 faces per block
        emitFace(vertices, indices, localCoords, face);
    }
}

void ChunkMeshSystem::emitFace(std::vector<Vertex> &vertices, std::vector<uint> &indices, const glm::ivec3 &localCoords,
                               uint face) {
    uint offset = vertices.size();
    for (uint i = 0; i < 4; i++) { // 4 vertices per face
        const glm::ivec3& cubeVertex = CUBE_VERTICES[CUBE_INDICES[6 * face + UNIQUE_INDICES[i]]];
        const glm::vec2& uv = CUBE_UVS[i];
        const Vertex v {
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
