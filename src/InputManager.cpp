//
// Created by Phil Lan on 2023-10-31.
//

#include "InputManager.h"

InputManager::InputManager(GLFWwindow *window): window(window) {

}

int InputManager::getKey(int key) {
    return glfwGetKey(window, key);
}

glm::dvec2 InputManager::getCursorPos() {
    glm::dvec2 retVal;
    glfwGetCursorPos(window, &retVal.x, &retVal.y);
    return retVal;
}

int InputManager::getMouseButton(int mouseButton) {
    return glfwGetMouseButton(window, mouseButton);
}
