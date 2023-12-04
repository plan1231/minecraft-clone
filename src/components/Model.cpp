//
// Created by Phil Lan on 2023-11-06.
//
#include "Model.h"
void Model::render(ShaderPtr shaderPtr) const {
    if(texture != nullptr) {
        texture->use(0);
        shaderPtr->setInt("theTexture", 0);
    }

    mesh->draw();
}
