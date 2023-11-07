//
// Created by Phil Lan on 2023-11-06.
//

#include "ChunkTransformComponent.h"
#include <glm/gtx/transform.hpp>

void ChunkTransform::calculateTransform() {
    transform = glm::translate(position);
}
