//
// Created by Phil Lan on 2023-10-27.
//

#include "System.h"

System::System(entt::registry &registry, entt::dispatcher &dispatcher) :
        registry(registry),
        dispatcher(dispatcher),
        chunkManager(entt::locator<ChunkManager>::value()),
        inputManager(entt::locator<InputManager>::value()),
        threadPool(entt::locator<ThreadPool>::value())
{

}
