//
// Created by Phil Lan on 2023-11-22.
//

#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include "PerlinNoise.h"
#include "world/Chunk.h"

class TerrainGenerator {
public:
    void generateTerrain(Chunk &cc, const glm::ivec3 &chunkCoords);
private:
    PerlinNoise noise;
};

#endif //TERRAINGENERATOR_H
