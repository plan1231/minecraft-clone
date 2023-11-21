//
// Created by Phil Lan on 2023-10-27.
//

#include "Game.h"
#include "systems/RenderingSystem.h"
#include "systems/PlayerSystem.h"
#include "systems/ChunkMeshSystem.h"


#include "ChunkManager.h"

Game::Game(InputManager &inputManager): inputManager(inputManager) {
    entt::locator<InputManager>::emplace(inputManager);

    entt::locator<ChunkManager>::emplace(ChunkManager(registry));

    entt::locator<ChunkManager>::value().loadChunk({0, 0}, BlockType::DIRT);
    entt::locator<ChunkManager>::value().loadChunk({-1, 0}, BlockType::AIR);
    systems.push_back(new PlayerSystem(registry, dispatcher));
    systems.push_back(new ChunkMeshSystem(registry, dispatcher));
    systems.push_back(new RenderingSystem(registry, dispatcher));
}

void Game::update(float dt) {
    for (auto&system: systems) {
        system->update(dt);
    }
}
