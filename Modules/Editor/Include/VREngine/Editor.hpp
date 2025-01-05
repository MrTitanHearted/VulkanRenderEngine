#pragma once

#include <VREngine/Engine.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace vre {
    constexpr std::uint32_t FRAME_OVERLAP = 3u;

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Color;
    };

    class DeletionQueue {
       public:
        DeletionQueue()  = default;
        ~DeletionQueue() = default;

        void add(const std::function<void()> &deletor);
        void flush();

       private:
        std::vector<std::function<void()>> m_Deletors;
    };

    struct FrameData {
        vk::CommandPool   CommandPool;
        vk::CommandBuffer MainCommandBuffer;

        vk::Semaphore SwapchainSemaphore;
        vk::Semaphore RenderSemaphore;
        vk::Fence     RenderFence;

        DeletionQueue DeletionQueue;
    };

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

        std::array<FrameData, FRAME_OVERLAP> m_Frames;
        std::uint32_t                        m_FrameNumber;

        vk::CommandPool   m_ImmCommandPool;
        vk::CommandBuffer m_ImmCommandBuffer;
        vk::Fence         m_ImmFence;

        vk::PipelineLayout         m_TrianglePipelineLayout;
        vk::Pipeline               m_TrianglePipeline;
        Vulkan::Buffer::Allocation m_TriangleIndexBuffer;
        Vulkan::Buffer::Allocation m_TriangleVertexBuffer;

        DeletionQueue m_MainDeletionQueue;

       private:
        void renderImGui();

        void draw();
        void drawGeometry(const vk::CommandBuffer &cmd, const vk::ImageView &target);
        void drawImGui(const vk::CommandBuffer &cmd, const vk::ImageView &target);
        void resize();

        void initImGui();
        void initTriangle();

        void submitImmediately(const std::function<void(const vk::CommandBuffer &cmd)> &function);

        FrameData &getCurrentFrame();

        void closeCallback(const WindowCloseEvent &event);
        void keyCallback(const WindowKeyEvent &event);
    };
}  // namespace vre