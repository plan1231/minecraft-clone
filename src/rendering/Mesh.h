//
// Created by Phil Lan on 2023-10-28.
//

#ifndef MINECRAFT_CLONE_MESH_H
#define MINECRAFT_CLONE_MESH_H

#include "Geometry.h"
#include "Texture.h"
#include "Shader.h"
class Mesh {
public:
    Geometry* geometry;
    Texture* texture;
    void render(ShaderPtr& shaderPtr) const;
};


#endif //MINECRAFT_CLONE_MESH_H
