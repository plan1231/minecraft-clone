//
// Created by Phil Lan on 2023-10-31.
//

#ifndef MINECRAFT_CLONE_INPUTMANAGER_H
#define MINECRAFT_CLONE_INPUTMANAGER_H
#include <glad/glad.h> // include glad so it'll shut up about glfw3.h being included
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputManager {

    GLFWwindow *window = nullptr;

public:
    InputManager(GLFWwindow *window);
    int getKey(int key);
    int getMouseButton(int mouseButton);
    glm::dvec2 getCursorPos();
};


#endif //MINECRAFT_CLONE_INPUTMANAGER_H
