#ifndef SNEKON_INPUT_H
#define SNEKON_INPUT_H

#include "GLFW/glfw3.h"

enum class KeyCode: int {
    A = GLFW_KEY_A,
    D = GLFW_KEY_D,
    S = GLFW_KEY_S,
    W = GLFW_KEY_W,
    Right = GLFW_KEY_RIGHT,
    Left = GLFW_KEY_LEFT,
    Down = GLFW_KEY_DOWN,
    Up = GLFW_KEY_UP,
};

enum class KeyState: int {
    Release = GLFW_RELEASE,
    Press = GLFW_PRESS,
    Hold = GLFW_REPEAT
};

#endif //SNEKON_INPUT_H
