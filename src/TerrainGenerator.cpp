//
// Created by Phil Lan on 2023-11-22.
//

#include "TerrainGenerator.h"
constexpr int BASE_HEIGHT = 100;
constexpr double PERLIN_AMPLITUDE = 20.0;
void TerrainGenerator::generateTerrain(ChunkComponent& cc, const glm::ivec2& chunkCoords) {
    std::vector<int> heightMap(CHUNK_LENGTH * CHUNK_LENGTH);
    for(int x = 0; x < CHUNK_LENGTH; x++) {
        for(int z = 0; z < CHUNK_LENGTH; z++) {
            heightMap[x * CHUNK_LENGTH + z] = BASE_HEIGHT + PERLIN_AMPLITUDE * noise.noise(
                0.1 * (x + chunkCoords.x * CHUNK_LENGTH),
                0.5,
                0.1 * (z + chunkCoords.y * CHUNK_LENGTH));
        }
    }
    for(int y = 0; y < CHUNK_HEIGHT; y++) {
        for(int x = 0; x < CHUNK_LENGTH; x++) {
            for(int z = 0; z < CHUNK_LENGTH; z++) {
                (*cc.blockTypes)[y * CHUNK_LENGTH * CHUNK_LENGTH + x * CHUNK_LENGTH + z] = y <= heightMap[x * CHUNK_LENGTH + z] ? BlockType::DIRT : BlockType::AIR;
            }
        }
    }
}
