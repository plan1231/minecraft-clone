//
// Created by Phil Lan on 2023-11-22.
//

#include "TerrainGenerator.h"
constexpr int BASE_HEIGHT = 100;
constexpr double PERLIN_AMPLITUDE = 20.0;
void TerrainGenerator::generateTerrain(Chunk& cc, const glm::ivec3& chunkCoords) {
    std::vector<int> heightMap(CHUNK_ACTUAL_SIZE * CHUNK_ACTUAL_SIZE);
    for(int x = 0; x < CHUNK_ACTUAL_SIZE; x++) {
        for(int z = 0; z < CHUNK_ACTUAL_SIZE; z++) {
            int globalX = chunkCoords.x * CHUNK_SIZE + x - 1;
            int globalZ = chunkCoords.z * CHUNK_SIZE + z - 1;

            heightMap[x * CHUNK_ACTUAL_SIZE + z] = BASE_HEIGHT + PERLIN_AMPLITUDE * noise.noise(
                0.1 * globalX,
                0.5,
                0.1 * globalZ
            );
        }
    }
    for(int y = 0; y < CHUNK_ACTUAL_SIZE; y++) {
        for(int x = 0; x < CHUNK_ACTUAL_SIZE; x++) {
            for(int z = 0; z < CHUNK_ACTUAL_SIZE; z++) {
                int globalY = (y - 1) + chunkCoords.y * CHUNK_SIZE;
                cc.blockTypes[y * CHUNK_ACTUAL_SIZE * CHUNK_ACTUAL_SIZE + x * CHUNK_ACTUAL_SIZE + z] = globalY <= heightMap[x * CHUNK_ACTUAL_SIZE + z] ? BlockType::DIRT : BlockType::AIR;
            }
        }
    }
}
