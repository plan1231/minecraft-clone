//
// Created by Phil Lan on 2023-11-28.
//

#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "System.h"

class PhysicsSystem: public System {
public:
    PhysicsSystem(entt::registry &registry, entt::dispatcher &dispatcher);
    void update(float dt) override;
};



#endif //PHYSICSSYSTEM_H
