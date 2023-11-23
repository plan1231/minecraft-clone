//
// Created by Phil Lan on 2023-10-27.
//

#include "Game.h"
#include "systems/RenderingSystem.h"
#include "systems/PlayerSystem.h"
#include "systems/ChunkMeshSystem.h"


#include "ChunkManager.h"
#include "systems/ChunkLoadingSystem.h"

Game::Game(InputManager &inputManager): inputManager(inputManager) {
    entt::locator<InputManager>::emplace(inputManager);
    entt::locator<ChunkManager>::emplace(ChunkManager(registry));

    systems.push_back(new PlayerSystem(registry, dispatcher));
    systems.push_back(new ChunkLoadingSystem(registry, dispatcher));
    systems.push_back(new ChunkMeshSystem(registry, dispatcher));
    systems.push_back(new RenderingSystem(registry, dispatcher));
}

void Game::update(float dt) {
    for (auto &system: systems) {
        system->update(dt);
    }
}
