//
// Created by Phil Lan on 2023-10-27.
//

#include "Application.h"
#include <iostream>


Application::Application() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    // glfw window creation
    // --------------------
    window = glfwCreateWindow(800, 600, "Minecraft Clone", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwSetWindowUserPointer(window, this);
    // Disable cursor and use raw mouse motion, better for manipulating a 3d camera as per glfw docs
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(1);
    }
    inputManager = std::make_unique<InputManager>(window);
    game = std::make_unique<Game>(*inputManager);

}

void Application::run() {
    glfwSetTime(0);
    float lastTime = 0;
    while(!stopRequested) {
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.7f, 0.877f, 0.917f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentTime = static_cast<float>(glfwGetTime());
        float delta = currentTime - lastTime;
        game->update(delta);

        glfwSwapBuffers(window);
        glfwPollEvents();

        lastTime = currentTime;
        stopRequested |= glfwWindowShouldClose(window);

    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
