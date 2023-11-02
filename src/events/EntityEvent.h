//
// Created by Phil Lan on 2023-11-01.
//

#ifndef MINECRAFT_CLONE_ENTITYEVENT_H
#define MINECRAFT_CLONE_ENTITYEVENT_H

#include "Event.h"
#include <entt/entt.hpp>

struct EntityEvent : public Event {
    entt::entity entity;

    EntityEvent(entt::entity entity) : entity(entity) {}
};

#endif //MINECRAFT_CLONE_ENTITYEVENT_H
