//
// Created by Phil Lan on 2023-10-29.
//

#ifndef MINECRAFT_CLONE_MESHCOMPONENT_H
#define MINECRAFT_CLONE_MESHCOMPONENT_H
#include "rendering/Mesh.h"
#include "rendering/Texture.h"
#include "rendering/Shader.h"
#include <memory>

struct Model {
    MeshPtr mesh;
    TexturePtr texture;
    void render(ShaderPtr shaderPtr) const;
};
#endif //MINECRAFT_CLONE_MESHCOMPONENT_H
