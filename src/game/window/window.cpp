#include "window.h"
#include "../utility/utility.h"

auto Window::init() -> std::expected<Window, WindowError> {
    Window window {};

    if (!glfwInit()) {
        return std::unexpected(WindowError::GlfwInit);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window.m_window = glfwCreateWindow(512, 512, "snekon", nullptr, nullptr);
    if (!window.m_window) {
        return std::unexpected(WindowError::GlfwWindow);
    }

    return window;
}

auto Window::deinit() -> void {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

auto Window::update() -> void {
    glfwPollEvents();
}

auto Window::closed() -> bool {
    return glfwWindowShouldClose(m_window);
}

auto Window::handle() -> GLFWwindow* {
    return m_window;
}

Window::~Window() { deinit(); }

auto Window::key(KeyCode code) -> KeyState {
    return static_cast<KeyState>(
        glfwGetKey(m_window, static_cast<int>(code))
    );
}
