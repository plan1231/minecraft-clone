//
// Created by Phil Lan on 2023-10-28.
//

#include "Mesh.h"

void Mesh::render(ShaderPtr& shaderPtr) const {
    texture->use(0);
    shaderPtr->setInt("ourTexture", 0);
    geometry->draw();
}
