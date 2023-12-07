//
// Created by Phil Lan on 2023-12-05.
//
#pragma once
#include <glm/glm.hpp>

// downwards acceleration due to gravity
constexpr float g = -32;
// Terminal speed in the y direction
constexpr float TERM_SPEED_Y = 60.0f;
// Terminal speed on the xz plane
constexpr float TERM_SPEED_XZ= 4.2f;

constexpr float DRAG_DEACCELERATION = 21.0f;


constexpr float MOVEMENT_ACCELERATION = 30.0f;
constexpr float JUMP_HEIGHT = 1.3f;