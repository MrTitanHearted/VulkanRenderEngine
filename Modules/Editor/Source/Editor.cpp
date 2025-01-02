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

        Vulkan::Image::TransitionLayout(cmd, swapchainImage, vk::ImageLayout::eGeneral);

        cmd.clearColorImage(
            swapchainImage,
            vk::ImageLayout::eGeneral,
            std::array<float, 4>{0.2f, 0.3f, 0.3f, 1.0f},
            {Vulkan::Image::GetColorRange()});

        Vulkan::Image::TransitionLayout(
            cmd,
            swapchainImage,
            vk::ImageLayout::eGeneral,
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

    FrameData &Editor::getCurrentFrame() {
        return m_Frames[m_FrameNumber % FRAME_OVERLAP];
    }

    void Editor::closeCallback(const WindowCloseEvent &event) {
        m_IsRunning = false;
    }

    void Editor::keyCallback(const WindowKeyEvent &event) {
        if (event.Action != KeyAction::ePress) return;

        if (event.Key == KeyCode::eF)
            if (Window::IsFullscreen())
                Window::MakeWindowed();
            else
                Window::MakeFullscreen();
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