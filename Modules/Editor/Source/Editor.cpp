#include <VREngine/Editor.hpp>

namespace vre {
    Editor::Editor()
        : m_IsInitialized{false} {}

    Editor::~Editor() {
        VRE_ASSERT(!m_IsInitialized, "vre::Editor must be released before closing");
    }

    void Editor::init() {
        DVRE_ASSERT(!m_IsInitialized, "vre::Editor must be released before initialized");
        DLOG_INFO("Initializing vre::Editor");

        m_WindowCloseCallbackHandle = EventObserver::AddCallback<WindowCloseEvent>(this, &Editor::closeCallback);
        m_WindowKeyCallbackHandle   = EventObserver::AddCallback<WindowKeyEvent>(this, &Editor::keyCallback);

        m_Device           = Vulkan::Context::GetDevice();
        m_Surface          = Vulkan::Context::GetSurface();
        m_QueueFamilyIndex = Vulkan::Context::GetQueueFamilyIndex();
        m_GraphicsQueue    = Vulkan::Context::GetGraphicsQueue();
        m_ComputeQueue     = Vulkan::Context::GetComputeQueue();
        m_PresentQueue     = Vulkan::Context::GetPresentQueue();

        resize();

        m_VmaAllocator = Vulkan::Context::GetVmaAllocator();

        for (FrameData &frame : m_Frames) {
            frame.CommandPool = Vulkan::CommandPool::CreateResetCommandBuffer(
                m_QueueFamilyIndex,
                m_Device);
            frame.MainCommandBuffer = Vulkan::CommandBuffer::AllocatePrimary(
                frame.CommandPool,
                m_Device);
            frame.SwapchainSemaphore = Vulkan::Semaphore::Create(m_Device);
            frame.RenderSemaphore    = Vulkan::Semaphore::Create(m_Device);
            frame.RenderFence        = Vulkan::Fence::CreateSignaled(m_Device);
        }

        m_MainDeletionQueue.add([this] {
            for (FrameData &frame : m_Frames) {
                m_Device.destroy(frame.CommandPool);
                m_Device.destroy(frame.SwapchainSemaphore);
                m_Device.destroy(frame.RenderSemaphore);
                m_Device.destroy(frame.RenderFence);
            }
        });

        m_ImmCommandPool   = Vulkan::CommandPool::Create(m_QueueFamilyIndex, m_Device);
        m_ImmCommandBuffer = Vulkan::CommandBuffer::AllocatePrimary(m_ImmCommandPool, m_Device);
        m_ImmFence         = Vulkan::Fence::CreateSignaled(m_Device);

        m_MainDeletionQueue.add([this] {
            m_Device.destroy(m_ImmCommandPool);
            m_Device.destroy(m_ImmFence);
        });

        initImGui();
        initTriangle();

        m_IsInitialized = true;
    }

    void Editor::run() {
        DVRE_ASSERT(m_IsInitialized, "vre::Editor must be initialized");

        m_IsRunning      = true;
        m_StopProcessing = false;

        while (m_IsRunning) {
            Window::PollEvents();

            if (m_StopProcessing) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100u));
                m_StopProcessing = false;
                continue;
            }

            ImGui_ImplGlfw_NewFrame();
            ImGui_ImplVulkan_NewFrame();
            ImGui::NewFrame();

            renderImGui();

            ImGui::EndFrame();
            ImGui::Render();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            draw();
        }
    }

    void Editor::release() {
        DVRE_ASSERT(m_IsInitialized, "vre::Editor must be initialized before releasing");
        DLOG_INFO("Releasing vre::Editor");

        VRE_VK_CHECK(m_Device.waitIdle());

        m_MainDeletionQueue.flush();

        EventObserver::RemoveCallback<WindowCloseEvent>(m_WindowCloseCallbackHandle);
        EventObserver::RemoveCallback<WindowKeyEvent>(m_WindowKeyCallbackHandle);

        m_IsInitialized = false;
    }

    void Editor::renderImGui() {
        ImGui::ShowDemoWindow();
    }

    void Editor::draw() {
        FrameData &frame = getCurrentFrame();

        VRE_VK_CHECK(m_Device.waitForFences(frame.RenderFence, vk::True, 1000000000u));
        frame.DeletionQueue.flush();

        std::uint32_t swapchainImageIndex = 0u;
        {
            auto [result, index] =
                m_Device.acquireNextImageKHR(m_Swapchain, 1000000000u, frame.SwapchainSemaphore);
            if (result == vk::Result::eErrorOutOfDateKHR) {
                resize();
                return;
            } else if (result != vk::Result::eSuboptimalKHR)
                VRE_VK_CHECK(result);

            swapchainImageIndex = index;
        }

        m_Device.resetFences(frame.RenderFence);

        const vk::Image     &swapchainImage     = m_SwapchainImages[swapchainImageIndex];
        const vk::ImageView &swapchainImageView = m_SwapchainImageViews[swapchainImageIndex];

        const vk::CommandBuffer &cmd = frame.MainCommandBuffer;
        cmd.reset();
        Vulkan::CommandBuffer::BeginOneTimeSubmit(cmd);

        /*Vulkan::Image::TransitionLayout(cmd, swapchainImage, vk::ImageLayout::eGeneral);

        cmd.clearColorImage(
            swapchainImage,
            vk::ImageLayout::eGeneral,
            std::array<float, 4>{0.2f, 0.3f, 0.3f, 1.0f},
            {Vulkan::Image::GetColorRange()});*/

        Vulkan::Image::TransitionLayout(
            cmd,
            swapchainImage,
            // vk::ImageLayout::eGeneral,
            vk::ImageLayout::eColorAttachmentOptimal);
        drawGeometry(cmd, swapchainImageView);
        drawImGui(cmd, swapchainImageView);
        Vulkan::Image::TransitionLayout(
            cmd,
            swapchainImage,
            vk::ImageLayout::eColorAttachmentOptimal,
            vk::ImageLayout::ePresentSrcKHR);

        Vulkan::CommandBuffer::End(cmd);

        Vulkan::Queue::Submit(
            {Vulkan::CommandBuffer::GetSubmitInfo(cmd)},
            {Vulkan::Semaphore::GetSubmitInfo(
                vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                frame.SwapchainSemaphore)},
            {Vulkan::Semaphore::GetSubmitInfo(
                vk::PipelineStageFlagBits2::eAllGraphics,
                frame.RenderSemaphore)},
            frame.RenderFence,
            m_GraphicsQueue);

        vk::Result presentResult = Vulkan::Queue::Present(
            frame.RenderSemaphore,
            m_Swapchain,
            swapchainImageIndex,
            m_PresentQueue);
        if (presentResult == vk::Result::eErrorOutOfDateKHR ||
            presentResult == vk::Result::eSuboptimalKHR) {
            resize();
            return;
        }
        VRE_VK_CHECK(presentResult);

        m_FrameNumber++;
    }

    void Editor::drawGeometry(const vk::CommandBuffer &cmd, const vk::ImageView &target) {
        vk::Viewport viewport{
            0.0f,
            float(m_SwapchainExtent.height),
            float(m_SwapchainExtent.width),
            -float(m_SwapchainExtent.height),
            0.0f,
            1.0f,
        };
        vk::Rect2D scissor{
            vk::Offset2D{0, 0},
            m_SwapchainExtent,
        };

        Vulkan::RenderPass::BeginRendering(
            cmd,
            m_SwapchainExtent,
            {Vulkan::RenderPass::GetAttachmentInfo(
                target,
                vk::ImageLayout::eColorAttachmentOptimal,
                vk::ClearColorValue{std::array<float, 4>{0.2f, 0.3f, 0.3f, 1.0f}})});
        cmd.setViewport(0, {viewport});
        cmd.setScissor(0, {scissor});

        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, m_TrianglePipeline);
        cmd.bindIndexBuffer(m_TriangleIndexBuffer.Buffer, 0U, vk::IndexType::eUint32);
        cmd.bindVertexBuffers(0u, {m_TriangleVertexBuffer.Buffer}, {0U});
        cmd.drawIndexed(3u, 1u, 0u, 0u, 0u);

        Vulkan::RenderPass::EndRendering(cmd);
    }

    void Editor::drawImGui(const vk::CommandBuffer &cmd, const vk::ImageView &target) {
        Vulkan::RenderPass::BeginRendering(
            cmd,
            m_SwapchainExtent,
            {Vulkan::RenderPass::GetAttachmentInfo(target, vk::ImageLayout::eColorAttachmentOptimal)});
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
        Vulkan::RenderPass::EndRendering(cmd);
    }

    void Editor::resize() {
        auto [width, height] = Window::GetSize();
        if (width == 0 || height == 0) {
            m_StopProcessing = true;
            return;
        }

        Vulkan::Context::Resize();

        m_Swapchain           = Vulkan::Context::GetSwapchain();
        m_SwapchainFormat     = Vulkan::Context::GetSwapchainFormat();
        m_SwapchainExtent     = Vulkan::Context::GetSwapchainExtent();
        m_SwapchainImages     = Vulkan::Context::GetSwapchainImages();
        m_SwapchainImageViews = Vulkan::Context::GetSwapchainImageViews();
    }

    void Editor::initImGui() {
        vk::DescriptorPool imGuiPool =
            Vulkan::DescriptorPool::CreateFreeDescriptorSets(
                1000,
                {
                    vk::DescriptorPoolSize{vk::DescriptorType::eSampler, 1000u},
                    vk::DescriptorPoolSize{vk::DescriptorType::eCombinedImageSampler, 1000u},
                    vk::DescriptorPoolSize{vk::DescriptorType::eSampledImage, 1000u},
                    vk::DescriptorPoolSize{vk::DescriptorType::eStorageImage, 1000u},
                    vk::DescriptorPoolSize{vk::DescriptorType::eUniformTexelBuffer, 1000u},
                    vk::DescriptorPoolSize{vk::DescriptorType::eStorageTexelBuffer, 1000u},
                    vk::DescriptorPoolSize{vk::DescriptorType::eUniformBuffer, 1000u},
                    vk::DescriptorPoolSize{vk::DescriptorType::eStorageBuffer, 1000u},
                    vk::DescriptorPoolSize{vk::DescriptorType::eUniformBufferDynamic, 1000u},
                    vk::DescriptorPoolSize{vk::DescriptorType::eStorageBufferDynamic, 1000u},
                    vk::DescriptorPoolSize{vk::DescriptorType::eInputAttachment, 1000u},
                },
                m_Device);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |=
            ImGuiConfigFlags_DockingEnable |
            ImGuiConfigFlags_ViewportsEnable;
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForVulkan(Window::GetGlfwWindow(), true);
        ImGui_ImplVulkan_InitInfo initInfo{
            .Instance                    = Vulkan::Context::GetInstance(),
            .PhysicalDevice              = Vulkan::Context::GetPhysicalDevice(),
            .Device                      = m_Device,
            .QueueFamily                 = m_QueueFamilyIndex,
            .Queue                       = m_GraphicsQueue,
            .DescriptorPool              = imGuiPool,
            .MinImageCount               = FRAME_OVERLAP,
            .ImageCount                  = FRAME_OVERLAP,
            .MSAASamples                 = VK_SAMPLE_COUNT_1_BIT,
            .UseDynamicRendering         = true,
            .PipelineRenderingCreateInfo = vk::PipelineRenderingCreateInfo{}
                                               .setColorAttachmentFormats({m_SwapchainFormat}),
        };

        ImGui_ImplVulkan_Init(&initInfo);
        ImGui_ImplVulkan_CreateFontsTexture();

        io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
        io.ConfigWindowsResizeFromEdges = false;

        m_MainDeletionQueue.add([imGuiPool = imGuiPool, this] {
            ImGui_ImplVulkan_DestroyFontsTexture();
            ImGui_ImplVulkan_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            m_Device.destroy(imGuiPool);
        });
    }

    void Editor::initTriangle() {
        std::vector<std::uint32_t> triangleIndices{0u, 1u, 2u};
        std::vector<Vertex>        triangleVertices{
            Vertex{glm::vec3{-0.5f, -0.5f, 0.0f}, glm::vec3{1.0f, 0.0f, 0.0f}},
            Vertex{glm::vec3{0.0f, 0.5f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f}},
            Vertex{glm::vec3{0.5f, -0.5f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}},
        };

        std::uint64_t triangleIndicesSize  = sizeof(std::uint32_t) * triangleIndices.size();
        std::uint64_t triangleVerticesSize = sizeof(Vertex) * triangleVertices.size();

        FileAsset shaderSource = FileAsset::FromPathBinary(
            fs::path("Assets") /
            fs::path("Shaders") /
            fs::path("Triangle.spv"));
        vk::ShaderModule shaderModule = Vulkan::Shader::CreateSPV(shaderSource, m_Device);

        m_TrianglePipelineLayout = Vulkan::PipelineLayout::Create({}, m_Device);
        m_TrianglePipeline =
            Vulkan::GraphicsPipeline::Builder{}
                .setVertexShader("vsmain", shaderModule)
                .setFragmentShader("fsmain", shaderModule)
                .setVertexLayout(Vulkan::VertexLayout{
                    vk::VertexInputBindingDescription{
                        0u,
                        sizeof(Vertex),
                        vk::VertexInputRate::eVertex,
                    },
                    {
                        vk::VertexInputAttributeDescription{0u, 0u, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, Position)},
                        vk::VertexInputAttributeDescription{1u, 0u, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, Color)},
                    },
                })
                .setInputTopology(vk::PrimitiveTopology::eTriangleList)
                .setPolygonMode(vk::PolygonMode::eFill)
                .setCullMode(vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise)
                .setNoDepthTest()
                .setNoMultisampling()
                .setNoBlending()
                .setColorAttachmentFormat(m_SwapchainFormat)
                .build(m_TrianglePipelineLayout, m_Device);

        m_Device.destroy(shaderModule);

        m_TriangleIndexBuffer = Vulkan::Buffer::Allocate(
            VMA_MEMORY_USAGE_GPU_ONLY,
            triangleIndicesSize,
            vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
            m_VmaAllocator);
        m_TriangleVertexBuffer = Vulkan::Buffer::Allocate(
            VMA_MEMORY_USAGE_GPU_ONLY,
            triangleVerticesSize,
            vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
            m_VmaAllocator);

        Vulkan::Buffer::Allocation staging = Vulkan::Buffer::AllocateMapped(
            VMA_MEMORY_USAGE_CPU_ONLY,
            triangleIndicesSize + triangleVerticesSize,
            vk::BufferUsageFlagBits::eTransferSrc,
            m_VmaAllocator);

        VmaAllocationInfo stagingAllocationInfo{};
        vmaGetAllocationInfo(staging.Allocator, staging.Allocation, &stagingAllocationInfo);
        void *data = stagingAllocationInfo.pMappedData;

        std::memcpy(data, triangleIndices.data(), triangleIndicesSize);
        std::memcpy((char *)data + triangleIndicesSize, triangleVertices.data(), triangleVerticesSize);

        submitImmediately([&, this](const vk::CommandBuffer &cmd) {
            Vulkan::CommandBuffer::CopyBufferToBuffer(cmd, staging, m_TriangleIndexBuffer, triangleIndicesSize);
            Vulkan::CommandBuffer::CopyBufferToBuffer(cmd, staging, m_TriangleVertexBuffer, triangleIndicesSize, 0U, triangleVerticesSize);
        });

        Vulkan::Buffer::Release(staging);

        m_MainDeletionQueue.add([this] {
            m_Device.destroy(m_TrianglePipelineLayout);
            m_Device.destroy(m_TrianglePipeline);
            Vulkan::Buffer::Release(m_TriangleIndexBuffer);
            Vulkan::Buffer::Release(m_TriangleVertexBuffer);
        });
    }

    void Editor::submitImmediately(const std::function<void(const vk::CommandBuffer &cmd)> &function) {
        m_Device.resetFences({m_ImmFence});
        m_Device.resetCommandPool(m_ImmCommandPool);

        const vk::CommandBuffer &cmd = m_ImmCommandBuffer;

        Vulkan::CommandBuffer::BeginOneTimeSubmit(cmd);
        function(cmd);
        Vulkan::CommandBuffer::End(cmd);

        Vulkan::Queue::Submit({Vulkan::CommandBuffer::GetSubmitInfo(cmd)}, m_ImmFence, m_GraphicsQueue);
        DVRE_VK_CHECK(m_Device.waitForFences({m_ImmFence}, vk::True, 1000000000U));
    }

    FrameData &Editor::getCurrentFrame() {
        return m_Frames[m_FrameNumber % FRAME_OVERLAP];
    }

    void Editor::closeCallback(const WindowCloseEvent &event) {
        m_IsRunning = false;
    }

    void Editor::keyCallback(const WindowKeyEvent &event) {
        if (event.Action != KeyAction::ePress) return;

        if (event.Key == KeyCode::eF) {
            ImGuiIO &io = ImGui::GetIO();

            if (Window::IsFullscreen()) {
                Window::MakeWindowed();
                io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            } else {
                Window::MakeFullscreen();
                io.ConfigFlags &= !ImGuiConfigFlags_ViewportsEnable;
            }
        }
    }

    void DeletionQueue::add(const std::function<void()> &deletor) {
        m_Deletors.push_back(deletor);
    }

    void DeletionQueue::flush() {
        for (std::int32_t i = m_Deletors.size() - 1; i >= 0; i--)
            m_Deletors[i]();
        m_Deletors.clear();
    }
}  // namespace vre