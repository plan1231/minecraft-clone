//
// Created by Phil Lan on 2023-10-27.
//

#ifndef MINECRAFT_CLONE_APPLICATION_H
#define MINECRAFT_CLONE_APPLICATION_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Game.h"

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;

class Application {
    std::unique_ptr<Game> game;

    bool stopRequested = false;
    GLFWwindow *window = nullptr;

    static void cursorPosCallback(GLFWwindow* window, double x, double y);


public:
    void run();
    Application();
};


#endif //MINECRAFT_CLONE_APPLICATION_H
