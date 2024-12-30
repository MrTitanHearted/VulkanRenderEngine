#pragma once

#include <VREngine/Engine.hpp>

namespace vre {
    class Editor {
       public:
        Editor();
        ~Editor();

        void init();
        void run();
        void release();

       private:
        bool m_IsInitialized;
        bool m_IsRunning;
        bool m_StopProcessing;

        EventCallbackHandle m_WindowCloseCallbackHandle;
        EventCallbackHandle m_WindowKeyCallbackHandle;

        vk::Device       m_Device;
        vk::SurfaceKHR   m_Surface;
        std::uint32_t    m_QueueFamilyIndex;
        vk::Queue        m_GraphicsQueue;
        vk::Queue        m_ComputeQueue;
        vk::Queue        m_PresentQueue;
        vk::SwapchainKHR m_Swapchain;
        vk::Format       m_SwapchainFormat;
        vk::Extent2D     m_SwapchainExtent;

        std::vector<vk::Image>     m_SwapchainImages;
        std::vector<vk::ImageView> m_SwapchainImageViews;

        VmaAllocator m_VmaAllocator;

       private:
        void resize();

        void closeCallback(const WindowCloseEvent &event);
        void keyCallback(const WindowKeyEvent &event);
    };
}  // namespace vre