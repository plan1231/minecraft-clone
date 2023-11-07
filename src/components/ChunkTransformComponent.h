//
// Created by Phil Lan on 2023-11-06.
//

#ifndef MINECRAFT_CLONE_CHUNKTRANSFORM_H
#define MINECRAFT_CLONE_CHUNKTRANSFORM_H
#include <glm/glm.hpp>

struct ChunkTransform {
    glm::ivec3 position;
    glm::mat4 transform;
    void calculateTransform();
};


#endif //MINECRAFT_CLONE_CHUNKTRANSFORM_H
