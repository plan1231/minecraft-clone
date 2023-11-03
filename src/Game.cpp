//
// Created by Phil Lan on 2023-10-27.
//

#include "Game.h"
#include "systems/RenderingSystem.h"
#include "systems/CameraSystem.h"

#include "components/MeshComponent.h"
#include "components/TransformationComponent.h"
#include "rendering/Geometry.h"
Game::Game(InputManager &inputManager): inputManager(inputManager) {
    systems.push_back(new CameraSystem(registry, dispatcher, inputManager));
    systems.push_back(new RenderingSystem(registry, dispatcher, inputManager));
    entt::entity square = registry.create();
    Geometry* g = new Geometry(VertexAttributes{
        VertexAttribute{3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0},
        VertexAttribute{2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),  (void*)(3 * sizeof(float))},
    }, GL_TRIANGLE_STRIP);
    g->bufferData({
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    }, {1, 0, 2, 3});
    Texture* t = new Texture(ASSETS_PATH"/textures/container.jpg");
    Mesh m{g, t};

    registry.emplace<MeshComponent>(square, m);
    registry.emplace<TransformationComponent>(square, glm::vec3(0.0, 0.0, 1.0), glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
}

void Game::update(float dt) {
    for(auto& system : systems) {
        system->update(dt);
    }
}
