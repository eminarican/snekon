#ifndef SNEKON_RENDERER_H
#define SNEKON_RENDERER_H

#include <expected>

#include "webgpu/webgpu.hpp"
#include "GLFW/glfw3.h"

extern "C" {
    WGPUSurface glfwGetWGPUSurface(WGPUInstance instance, GLFWwindow* window);
}

class RendererError;

class Renderer {
public:
    static auto init(GLFWwindow* window_handle) -> std::expected<Renderer, RendererError>;
    auto deinit() -> void;

    auto render(bool test) -> void;

    ~Renderer();

private:
    auto init_device(GLFWwindow* window_handle) -> std::expected<Renderer, RendererError>;
    auto deinit_device() -> void;

    auto init_swap_chain() -> std::expected<Renderer, RendererError>;
    auto deinit_swap_chain() -> void;

    auto init_render_pipeline() -> std::expected<Renderer, RendererError>;
    auto deinit_render_pipeline() -> void;

private:
    wgpu::Instance m_instance = nullptr;
    wgpu::Surface m_surface = nullptr;
    wgpu::Device m_device = nullptr;
    wgpu::Queue m_queue = nullptr;

    wgpu::TextureFormat m_swap_chain_format = wgpu::TextureFormat::Undefined;

    //std::unique_ptr<wgpu::ErrorCallback> m_errorCallbackHandle;

    wgpu::SwapChain m_swap_chain = nullptr;

    wgpu::ShaderModule m_shader_module = nullptr;
    wgpu::RenderPipeline m_pipeline = nullptr;
};

class RendererError {
public:
    enum Enum {
        WebgpuInstance,
        WebgpuDevice,
        WebgpuSwapChain,
    };

    constexpr RendererError() = default;
    constexpr RendererError(Enum e) : m_e(e) {}

    // Allows comparisons with ErrorCode constants.
    constexpr operator Enum() const { return m_e; }

    // Deleted to prevent if(c).
    explicit operator bool() const = delete;

    [[nodiscard]] std::string_view to_string() const {
        switch (m_e) {
            case WebgpuInstance: return "unable to get webgpu instance";
            case WebgpuDevice: return "unable to get webgpu device";
            case WebgpuSwapChain: return "unable to create swap chain";
        }
        return "";
    }

private:
    Enum m_e;
};

#endif //SNEKON_RENDERER_H
