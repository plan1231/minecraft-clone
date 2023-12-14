//
// Created by Phil Lan on 2023-10-27.
//

#include "Game.h"
#include "systems/RenderingSystem.h"
#include "systems/PlayerSystem.h"
#include "systems/PhysicsSystem.h"
#include "systems/ChunkSystem.h"

#include "factories.h"

#include "world/ChunkManager.h"
#include "GameEntities.h"

Game::Game() {
    entt::locator<ChunkManager>::emplace(ChunkManager{});
    entt::locator<GameEntities>::emplace(GameEntities {
        .player = makePlayer(registry)
    });

    makeCrosshair(registry);

    systems.push_back(new PlayerSystem(registry, dispatcher));
    systems.push_back(new ChunkSystem(registry, dispatcher));
    systems.push_back(new PhysicsSystem(registry, dispatcher));
    systems.push_back(new RenderingSystem(registry, dispatcher));
}

void Game::update(float dt) {
    for (auto &system: systems) {
        system->update(dt);
    }
}
