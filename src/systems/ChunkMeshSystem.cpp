//
// Created by Phil Lan on 2023-11-05.
//

#include "ChunkMeshSystem.h"

#include <iostream>

#include "components/Camera.h"
#include "components/Chunk.h"
#include "components/Model.h"
#include "components/Transform.h"
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
 *  6-------5
 *  |       |
 *  |       |
 *  |       |
 *  2-------1
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



void ChunkMeshSystem::update(float dt) {
    auto playerView = registry.view<Transform, Camera>();
    playerView.each([&](Transform&playerTransform, auto) {
        glm::ivec3 pChunkCoords = toChunk(playerTransform.position);
        registry.view<Chunk, Model, Transform>().each(
            [&](Chunk&chunk, Model&mesh, Transform&transform) {
                if (!chunk.modified) return;
                std::vector<Vertex> vertices;
                std::vector<uint> indices;
                for (int y = 1; y <= CHUNK_SIZE; y++) {
                    for (int x = 1; x <= CHUNK_SIZE; x++) {
                        for (int z = 1; z <= CHUNK_SIZE; z++) {
                            BlockType blockType = chunk.getBlock({x , y, z});
                            if (blockType == BlockType::AIR) continue;
                            emitBlock(vertices, indices, chunk, {x, y, z});
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
    const Chunk &chunk,
    const glm::ivec3 &localCoords) {
    BlockType currBlockType = chunk.getBlock(localCoords);

    // +z face
    if (BlockTypeInfo::get(chunk.getBlock({localCoords.x, localCoords.y, localCoords.z + 1})).isTransparent) {
        emitFace(vertices, indices, chunk, localCoords, currBlockType, Face::SOUTH);
    }

    // -z face
    if (BlockTypeInfo::get(chunk.getBlock({localCoords.x, localCoords.y, localCoords.z - 1})).isTransparent) {
        emitFace(vertices, indices, chunk, localCoords, currBlockType, Face::NORTH);
    }

    // +x face
    if (BlockTypeInfo::get(chunk.getBlock({localCoords.x + 1, localCoords.y, localCoords.z})).isTransparent) {
        emitFace(vertices, indices, chunk, localCoords, currBlockType, Face::EAST);
    }

    // -x face
    if (BlockTypeInfo::get(chunk.getBlock({localCoords.x - 1, localCoords.y, localCoords.z})).isTransparent) {
        emitFace(vertices, indices, chunk, localCoords, currBlockType, Face::WEST);
    }

    // +y face
    if (BlockTypeInfo::get(chunk.getBlock({localCoords.x, localCoords.y + 1, localCoords.z})).isTransparent) {
        emitFace(vertices, indices, chunk, localCoords, currBlockType, Face::UP);
    }

    // -y face
    if (BlockTypeInfo::get(chunk.getBlock({localCoords.x, localCoords.y - 1, localCoords.z})).isTransparent) {
        emitFace(vertices, indices, chunk, localCoords, currBlockType, Face::DOWN);
    }
}

std::array<int, 4> ChunkMeshSystem::getFaceAo(const Chunk &chunk, const BCoords& coords, Face face) {
    const int x = coords.x, y = coords.y, z = coords.z;
    int a, b, c, d, e, f, g, h;

    switch(face) {

        case Face::NORTH:
            a = BlockTypeInfo::get(chunk.getBlock({x + 1, y + 1, z - 1})).isTransparent;
            b = BlockTypeInfo::get(chunk.getBlock({x    , y + 1, z - 1})).isTransparent;
            c = BlockTypeInfo::get(chunk.getBlock({x - 1, y + 1, z - 1})).isTransparent;
            d = BlockTypeInfo::get(chunk.getBlock({x + 1, y    , z - 1})).isTransparent;
            e = BlockTypeInfo::get(chunk.getBlock({x - 1, y    , z - 1})).isTransparent;
            f = BlockTypeInfo::get(chunk.getBlock({x + 1, y - 1, z - 1})).isTransparent;
            g = BlockTypeInfo::get(chunk.getBlock({x    , y - 1, z - 1})).isTransparent;
            h = BlockTypeInfo::get(chunk.getBlock({x - 1, y - 1, z - 1})).isTransparent;
            break;
        case Face::SOUTH:
            a = BlockTypeInfo::get(chunk.getBlock({x - 1, y + 1, z + 1})).isTransparent;
            b = BlockTypeInfo::get(chunk.getBlock({x    , y + 1, z + 1})).isTransparent;
            c = BlockTypeInfo::get(chunk.getBlock({x + 1, y + 1, z + 1})).isTransparent;
            d = BlockTypeInfo::get(chunk.getBlock({x - 1, y    , z + 1})).isTransparent;
            e = BlockTypeInfo::get(chunk.getBlock({x + 1, y    , z + 1})).isTransparent;
            f = BlockTypeInfo::get(chunk.getBlock({x - 1, y - 1, z + 1})).isTransparent;
            g = BlockTypeInfo::get(chunk.getBlock({x    , y - 1, z + 1})).isTransparent;
            h = BlockTypeInfo::get(chunk.getBlock({x + 1, y - 1, z + 1})).isTransparent;
            break;
        case Face::UP:
            a = BlockTypeInfo::get(chunk.getBlock({x + 1, y + 1, z + 1})).isTransparent;
            b = BlockTypeInfo::get(chunk.getBlock({x    , y + 1, z + 1})).isTransparent;
            c = BlockTypeInfo::get(chunk.getBlock({x - 1, y + 1, z + 1})).isTransparent;
            d = BlockTypeInfo::get(chunk.getBlock({x + 1, y + 1, z    })).isTransparent;
            e = BlockTypeInfo::get(chunk.getBlock({x - 1, y + 1, z    })).isTransparent;
            f = BlockTypeInfo::get(chunk.getBlock({x + 1, y + 1, z - 1})).isTransparent;
            g = BlockTypeInfo::get(chunk.getBlock({x    , y + 1, z - 1})).isTransparent;
            h = BlockTypeInfo::get(chunk.getBlock({x - 1, y + 1, z - 1})).isTransparent;
            break;
        case Face::DOWN:
            a = BlockTypeInfo::get(chunk.getBlock({x - 1, y - 1, z + 1})).isTransparent;
            b = BlockTypeInfo::get(chunk.getBlock({x    , y - 1, z + 1})).isTransparent;
            c = BlockTypeInfo::get(chunk.getBlock({x + 1, y - 1, z + 1})).isTransparent;
            d = BlockTypeInfo::get(chunk.getBlock({x - 1, y - 1, z    })).isTransparent;
            e = BlockTypeInfo::get(chunk.getBlock({x + 1, y - 1, z    })).isTransparent;
            f = BlockTypeInfo::get(chunk.getBlock({x - 1, y - 1, z - 1})).isTransparent;
            g = BlockTypeInfo::get(chunk.getBlock({x    , y - 1, z - 1})).isTransparent;
            h = BlockTypeInfo::get(chunk.getBlock({x + 1, y - 1, z - 1})).isTransparent;
            break;
        case Face::EAST:
            a = BlockTypeInfo::get(chunk.getBlock({x + 1, y + 1, z + 1})).isTransparent;
            b = BlockTypeInfo::get(chunk.getBlock({x + 1, y + 1, z    })).isTransparent;
            c = BlockTypeInfo::get(chunk.getBlock({x + 1, y + 1, z - 1})).isTransparent;
            d = BlockTypeInfo::get(chunk.getBlock({x + 1, y    , z + 1})).isTransparent;
            e = BlockTypeInfo::get(chunk.getBlock({x + 1, y    , z - 1})).isTransparent;
            f = BlockTypeInfo::get(chunk.getBlock({x + 1, y - 1, z + 1})).isTransparent;
            g = BlockTypeInfo::get(chunk.getBlock({x + 1, y - 1, z   })).isTransparent;
            h = BlockTypeInfo::get(chunk.getBlock({x + 1, y - 1, z - 1})).isTransparent;
            break;
        case Face::WEST:
            a = BlockTypeInfo::get(chunk.getBlock({x - 1, y + 1, z - 1})).isTransparent;
            b = BlockTypeInfo::get(chunk.getBlock({x - 1, y + 1, z    })).isTransparent;
            c = BlockTypeInfo::get(chunk.getBlock({x - 1, y + 1, z + 1})).isTransparent;
            d = BlockTypeInfo::get(chunk.getBlock({x - 1, y    , z - 1})).isTransparent;
            e = BlockTypeInfo::get(chunk.getBlock({x - 1, y    , z + 1})).isTransparent;
            f = BlockTypeInfo::get(chunk.getBlock({x - 1, y - 1, z - 1})).isTransparent;
            g = BlockTypeInfo::get(chunk.getBlock({x - 1, y - 1, z    })).isTransparent;
            h = BlockTypeInfo::get(chunk.getBlock({x - 1, y - 1, z + 1})).isTransparent;
            break;
    }

   return std::array<int, 4>{d + f + g, g + h + e, b + c + e, d + a + b};
}

void ChunkMeshSystem::emitFace(std::vector<Vertex>&vertices,
    std::vector<uint> &indices,
    const Chunk &chunk,
    const glm::ivec3 &localCoords,
    BlockType blockType,
    Face face) {

    unsigned int offset = vertices.size();
    auto faceInt = static_cast<unsigned int>(face);
    std::array<int, 4> aoVals = getFaceAo(chunk, localCoords, face);
    // printf("ao vals %d %d %d %d\n", aoVals[0], aoVals[1], aoVals[2], aoVals[3]);
    for (unsigned int corner = 0; corner < 4; corner++) {
        // 4 vertices per face
        const glm::ivec3 &cubeVertex = CUBE_VERTICES[CUBE_INDICES[6 * faceInt + UNIQUE_INDICES[corner]]];
        const glm::vec2 &uv = BlockTypeInfo::get(blockType).textCoords(face, corner);
        const Vertex v{
            // subtract 1 to account for 1-block thick border of adjacent chunk blocks
            .x = localCoords.x + cubeVertex.x - 1,
            .y = localCoords.y + cubeVertex.y - 1,
            .z = localCoords.z + cubeVertex.z - 1,
            .u = uv.x,
            .v = uv.y,
            .aoLvl = aoVals[corner]
        };
        vertices.push_back(v);
    }
    indices.insert(indices.end(), {offset, offset + 1, offset + 2, offset, offset + 2, offset + 3});
}

ChunkMeshSystem::ChunkMeshSystem(entt::registry &registry, entt::dispatcher &dispatcher)
    : System(registry, dispatcher) {
}
