#ifndef SNEKON_WINDOW_H
#define SNEKON_WINDOW_H

#include <expected>

#include "webgpu/webgpu.hpp"
#include "GLFW/glfw3.h"

#include "input.h"

extern "C" {
    WGPUSurface glfwGetWGPUSurface(WGPUInstance instance, GLFWwindow* window);
}

class WindowError;

class Window {
public:
    static auto init() -> std::expected<Window, WindowError>;
    auto deinit() -> void;

    auto update() -> void;
    auto key(KeyCode code) -> KeyState;

    auto closed() -> bool;
    auto handle() -> GLFWwindow*;

    ~Window();

private:
    GLFWwindow* m_window;
};

class WindowError {
public:
    enum Enum {
        GlfwInit,
        GlfwWindow,
    };

    constexpr WindowError() = default;
    constexpr WindowError(Enum e) : m_e(e) {}

    // Allows comparisons with ErrorCode constants.
    constexpr operator Enum() const { return m_e; }

    // Deleted to prevent if(c).
    explicit operator bool() const = delete;

    [[nodiscard]] std::string_view to_string() const {
        switch (m_e) {
            case GlfwInit: return "unable to initialize glfw";
            case GlfwWindow: return "unable to create window";
        }
        return "";
    }

private:
    Enum m_e;
};

#endif //SNEKON_WINDOW_H
