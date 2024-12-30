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
        }
    }

    void Editor::release() {
        DVRE_ASSERT(m_IsInitialized, "vre::Editor must be initialized before releasing");
        DLOG_INFO("Releasing vre::Editor");

        m_Device.waitIdle();

        EventObserver::RemoveCallback<WindowCloseEvent>(m_WindowCloseCallbackHandle);
        EventObserver::RemoveCallback<WindowKeyEvent>(m_WindowKeyCallbackHandle);

        m_IsInitialized = false;
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
}  // namespace vre