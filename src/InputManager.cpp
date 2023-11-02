//
// Created by Phil Lan on 2023-10-31.
//

#include "InputManager.h"

InputManager::InputManager(GLFWwindow *window): window(window) {

}

int InputManager::getKey(int key) {
    return glfwGetKey(window, key);
}
