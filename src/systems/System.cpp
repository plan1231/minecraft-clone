//
// Created by Phil Lan on 2023-10-27.
//

#include "System.h"

System::System(entt::registry &registry, entt::dispatcher &dispatcher, InputManager &inputManager) :
        registry(registry),
        dispatcher(dispatcher),
        inputManager(inputManager)
{

}
