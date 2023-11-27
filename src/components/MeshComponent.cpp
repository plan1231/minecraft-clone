//
// Created by Phil Lan on 2023-11-06.
//
#include "MeshComponent.h"
void MeshComponent::render(ShaderPtr& shaderPtr) const {
    texture->use(0);
    shaderPtr->setInt("ourTexture", 0);
    mesh->draw();
}
