//
// Created by Phil Lan on 2023-10-29.
//

#ifndef MINECRAFT_CLONE_MESHCOMPONENT_H
#define MINECRAFT_CLONE_MESHCOMPONENT_H
#include "rendering/Geometry.h"
#include "rendering/Texture.h"
#include "rendering/Shader.h"

struct MeshComponent {
    Geometry* geometry;
    Texture* texture;
    void render(ShaderPtr& shaderPtr) const;
};
#endif //MINECRAFT_CLONE_MESHCOMPONENT_H
