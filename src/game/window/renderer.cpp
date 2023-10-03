#include "renderer.h"
#include "../utility/utility.h"

const char* g_shader_source = R"(
struct VertexInput {
	@location(0) position: vec2f,
	@location(1) color: vec3f,
};

struct VertexOutput {
	@builtin(position) position: vec4f,
	@location(0) color: vec3f,
};

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
	var out: VertexOutput;

	out.position = vec4f(in.position, 0.0, 1.0);
	out.color = in.color;

	return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
	return vec4f(in.color, 1.0);
}
)";

auto Renderer::init(GLFWwindow* window_handle) -> std::expected<Renderer, RendererError> {
    Renderer renderer;
    try_expected_(renderer.init_device(window_handle))
    try_expected_(renderer.init_swap_chain())
    //try_exp_(init_render_pipeline())
    return renderer;
}

auto Renderer::deinit() -> void {
    deinit_device();
    deinit_swap_chain();
    // deinit_render_pipeline();
}

auto Renderer::render(bool test) -> void {
    wgpu::TextureView nextTexture = m_swap_chain.getCurrentTextureView();
    if (!nextTexture) {
        std::cerr << "Cannot acquire next swap chain texture" << std::endl;
    }

    WGPUCommandEncoderDescriptor commandEncoderDesc {
        .label = "Command Encoder",
    };
    wgpu::CommandEncoder encoder = m_device.createCommandEncoder(commandEncoderDesc);

    WGPURenderPassColorAttachment renderPassColorAttachment {
        .view = nextTexture,
        .resolveTarget = nullptr,
        .loadOp =  wgpu::LoadOp::Clear,
        .storeOp = wgpu::StoreOp::Store,
    };

    if (test) {
        renderPassColorAttachment
            .clearValue = wgpu::Color{ 1.0, 1.0, 0.0, 0.5 };
    } else {
        renderPassColorAttachment
            .clearValue = wgpu::Color{ 0.0, 1.0, 0.0, 0.5 };
    }

    WGPURenderPassDescriptor renderPassDesc {
        .colorAttachmentCount = 1,
        .colorAttachments = &renderPassColorAttachment,
        .depthStencilAttachment = nullptr,
        .timestampWriteCount = 0,
        .timestampWrites = nullptr,
    };
    wgpu::RenderPassEncoder renderPass = encoder.beginRenderPass(renderPassDesc);
    renderPass.end();

    nextTexture.release();

    WGPUCommandBufferDescriptor cmdBufferDescriptor {
        .label = "Command buffer",
    };
    wgpu::CommandBuffer command = encoder.finish(cmdBufferDescriptor);
    m_queue.submit(1, &command);

    m_swap_chain.present();
}

Renderer::~Renderer() { deinit(); }

auto Renderer::init_device(GLFWwindow* window_handle) -> std::expected<Renderer, RendererError> {
    WGPUInstanceDescriptor instanceDescriptor {};
    m_instance = wgpuCreateInstance(&instanceDescriptor);
    if (!m_instance) {
        return std::unexpected(RendererError::WebgpuInstance);
    }

    m_surface = glfwGetWGPUSurface(m_instance, window_handle);

    WGPURequestAdapterOptions adapterOpts {
        .compatibleSurface = m_surface,
    };
    wgpu::Adapter adapter = m_instance.requestAdapter(adapterOpts);

    wgpu::SupportedLimits supportedLimits {};
    adapter.getLimits(&supportedLimits);

    wgpu::RequiredLimits requiredLimits = wgpu::Default;

    WGPUDeviceDescriptor deviceDescriptor {
        .label = "My Device",
        .requiredFeaturesCount = 0,
        //.requiredLimits = &requiredLimits,
        .defaultQueue {
            .label = "The default queue",
        },
    };
    m_device = adapter.requestDevice(deviceDescriptor);
    if (!m_device) {
        return std::unexpected(RendererError::WebgpuDevice);
    }

    adapter.release();

    //m_errorCallbackHandle = m_device.setUncapturedErrorCallback([]
    //(wgpu::ErrorType type, char const* message) {
    //    std::cout << "Device error: type " << type;
    //    if (message) std::cout << " (message: " << message << ")";
    //    std::cout << std::endl;
    //});

    m_queue = m_device.getQueue();

    m_swap_chain_format = m_surface.getPreferredFormat(adapter);

    return {};
}

auto Renderer::deinit_device() -> void {
    m_queue.release();
    m_device.release();
    m_surface.release();
    m_instance.release();
}

auto Renderer::init_swap_chain() -> std::expected<Renderer, RendererError> {
    WGPUSwapChainDescriptor swapChainDescriptor {
        .usage = wgpu::TextureUsage::RenderAttachment,
        .format = m_swap_chain_format,
        .width = 512,
        .height = 512,
        .presentMode = wgpu::PresentMode::Fifo,
    };
    m_swap_chain = m_device.createSwapChain(m_surface, swapChainDescriptor);

    if (!m_swap_chain) {
        return std::unexpected(RendererError::WebgpuSwapChain);
    }

    return {};
}

auto Renderer::deinit_swap_chain() -> void {
    m_swap_chain.release();
}

auto Renderer::init_render_pipeline() -> std::expected<Renderer, RendererError> {
    WGPUShaderModuleWGSLDescriptor shaderCodeDesc {
        .chain {
            .next = nullptr,
            .sType = wgpu::SType::ShaderModuleWGSLDescriptor,
        },
        .code = g_shader_source,
    };

    WGPUShaderModuleDescriptor shaderDesc {
        .nextInChain = &shaderCodeDesc.chain,
    };

    m_shader_module = wgpuDeviceCreateShaderModule(m_device, &shaderDesc);

    WGPUBlendState blendState {
        .color {
            .operation = wgpu::BlendOperation::Add,
            .srcFactor = wgpu::BlendFactor::SrcAlpha,
            .dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha,
        },
        .alpha {
            .operation = wgpu::BlendOperation::Add,
            .srcFactor = wgpu::BlendFactor::Zero,
            .dstFactor = wgpu::BlendFactor::One,
        },
    };

    WGPUColorTargetState colorTarget {
        .format = m_swap_chain_format,
        .blend = &blendState,
        .writeMask = wgpu::ColorWriteMask::All,
    };

    WGPUFragmentState fragmentState {
        .module = m_shader_module,
        .entryPoint = "fs_main",
        .constantCount = 0,
        .constants = nullptr,
        .targetCount = 1,
        .targets = &colorTarget,
    };

    WGPURenderPipelineDescriptor pipelineDesc {
        .layout = nullptr,
        .vertex {
            .module = m_shader_module,
            .entryPoint = "vs_main",
            .constantCount = 0,
            .constants = nullptr,
            //.bufferCount = static_cast<uint32_t>(vertexBufferLayouts.size()),
            //.buffers = vertexBufferLayouts.data(),
        },
        .primitive {
            .topology = wgpu::PrimitiveTopology::TriangleList,
            .stripIndexFormat = wgpu::IndexFormat::Undefined,
            .frontFace = wgpu::FrontFace::CCW,
            .cullMode = wgpu::CullMode::None,
        },
        .depthStencil = nullptr,
        .multisample {
            .count = 1,
            .mask = ~0u,
            .alphaToCoverageEnabled = false,
        },
        .fragment = &fragmentState,
    };
    wgpu::RenderPipeline pipeline = m_device.createRenderPipeline(pipelineDesc);

    return {};
}

auto Renderer::deinit_render_pipeline() -> void {
    m_pipeline.release();
    m_shader_module.release();
    //m_bind_group_layout.release();
}
