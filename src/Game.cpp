//
// Created by Phil Lan on 2023-10-27.
//

#include "Game.h"
#include "systems/RenderingSystem.h"
#include "systems/PlayerSystem.h"
#include "systems/ChunkMeshSystem.h"

#include "components/MeshComponent.h"
#include "components/TransformComponent.h"
#include "components/ChunkComponent.h"
#include "rendering/Geometry.h"
Game::Game(InputManager &inputManager): inputManager(inputManager) {
    systems.push_back(new PlayerSystem(registry, dispatcher, inputManager));
    systems.push_back(new ChunkMeshSystem(registry, dispatcher, inputManager));
    systems.push_back(new RenderingSystem(registry, dispatcher, inputManager));
//    entt::entity square = registry.create();
    Geometry* g = new Geometry(VertexAttributes{
        VertexAttribute{3, GL_INT, GL_FALSE, 2 * sizeof(float) + 3 * sizeof(unsigned int), 0},
        VertexAttribute{2, GL_FLOAT, GL_FALSE, 2 * sizeof(float) + 3 * sizeof(unsigned int),  (void*)(3 * sizeof(unsigned int))},
    }, GL_TRIANGLES);

    Texture* t = new Texture(ASSETS_PATH"/textures/container.jpg");
    MeshComponent m{g, t};
//
//    registry.emplace<MeshComponent>(square, m);
//    registry.emplace<TransformComponent>(square, glm::vec3(0.0, 0.0, 1.0), glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));

    entt::entity chunk = registry.create();
    registry.emplace<ChunkComponent>(chunk);
    registry.emplace<MeshComponent>(chunk, m);
    registry.emplace<TransformComponent>(chunk, glm::vec3(0.0, 0.0, 1.0), glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
}

void Game::update(float dt) {
    for(auto& system : systems) {
        system->update(dt);
    }
}
