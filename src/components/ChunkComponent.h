//
// Created by Phil Lan on 2023-11-03.
//

#ifndef MINECRAFT_CLONE_CHUNKCOMPONENT_H
#define MINECRAFT_CLONE_CHUNKCOMPONENT_H
#include <glm/glm.hpp>
#define CHUNK_LENGTH 16
struct ChunkComponent {
    // lower corner
    glm::ivec3 position;
    bool sameType;
    bool modified = true;
};
#endif //MINECRAFT_CLONE_CHUNKCOMPONENT_H
