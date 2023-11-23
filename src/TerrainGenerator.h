//
// Created by Phil Lan on 2023-11-22.
//

#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include "PerlinNoise.h"
#include "components/ChunkComponent.h"

class TerrainGenerator {
public:
    void generateTerrain(ChunkComponent &cc, const glm::ivec2 &chunkCoords);
private:
    PerlinNoise noise;
};

#endif //TERRAINGENERATOR_H
