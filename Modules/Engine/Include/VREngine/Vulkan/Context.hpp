#pragma once

#include <VREngine/Window.hpp>
#include <VREngine/Vulkan/Types.hpp>

namespace vre::Vulkan {
    class Context {
       public:
        struct Settings {
            std::vector<vk::Format>         PreferredSurfaceFormats{vk::Format::eB8G8R8A8Unorm, vk::Format::eB8G8R8A8Srgb};
            std::vector<vk::PresentModeKHR> PreferredPresentModes{vk::PresentModeKHR::eMailbox, vk::PresentModeKHR::eFifoRelaxed};
            std::uint32_t                   PreferredImageCount{3u};
            vk::ImageUsageFlags             SurfaceUsageFlags{0u};
        };

       public:
        static void Initialize(const Settings &settings);
        static void Shutdown();

        static bool IsInitialized();

        static void Resize();

        static vk::Instance               GetInstance();
        static vk::DebugUtilsMessengerEXT GetDebugUtilsMessenger();
        static vk::SurfaceKHR             GetSurface();
        static vk::PhysicalDevice         GetPhysicalDevice();
        static vk::Device                 GetDevice();
        static std::uint32_t              GetQueueFamilyIndex();
        static vk::Queue                  GetGraphicsQueue();
        static vk::Queue                  GetComputeQueue();
        static vk::Queue                  GetPresentQueue();
        static vk::SwapchainKHR           GetSwapchain();
        static vk::Format                 GetSwapchainFormat();
        static vk::Extent2D               GetSwapchainExtent();
        static vk::PresentModeKHR         GetSwapchainPresentMode();
        static vk::ImageUsageFlags        GetSwapchainUsageFlags();
        static std::vector<vk::Image>     GetSwapchainImages();
        static std::vector<vk::ImageView> GetSwapchainImageViews();
        static VmaAllocator               GetVmaAllocator();

       private:
        static vk::Instance               g_Instance;
        static vk::DebugUtilsMessengerEXT g_DebugMessenger;
        static vk::SurfaceKHR             g_Surface;
        static vk::PhysicalDevice         g_PhysicalDevice;
        static vk::Device                 g_Device;
        static std::uint32_t              g_QueueFamilyIndex;
        static std::uint32_t              g_QueueFamilyQueueCount;
        static vk::Queue                  g_GraphicsQueue;
        static vk::Queue                  g_ComputeQueue;
        static vk::Queue                  g_PresentQueue;
        static vk::SwapchainKHR           g_Swapchain;
        static vk::SurfaceFormatKHR       g_SwapchainFormat;
        static vk::Extent2D               g_SwapchainExtent;
        static vk::PresentModeKHR         g_SwapchainPresentMode;
        static vk::ImageUsageFlags        g_SwapchainUsageFlags;
        static std::uint32_t              g_SwapchainImageCount;
        static std::vector<vk::Image>     g_SwapchainImages;
        static std::vector<vk::ImageView> g_SwapchainImageViews;
        static VmaAllocator               g_VmaAllocator;

        static bool    g_IsInitialized;
        static Context g_State;

       private:
        Context() = default;
        ~Context();

        static void SelectPhysicalDevice(const std::vector<const char *> &requiredExtensions, const Settings &settings);
        static void SelectQueueFamilyIndex();
        static void SelectSwapchainImageCount(const Settings &settings);
        static void SelectSwapchainFormat(const Settings &settings);
        static void SelectSwapchainPresentMode(const Settings &settings);

        static void CreateSwapchain();
        static void ReleaseSwapchain();

        static bool CheckInstanceLayerSupport(const std::vector<const char *> &requiredLayers);
        static bool CheckPhysicalDeviceFeatureSupport(const vk::PhysicalDevice &physicalDevice);
        static bool CheckPhysicalDeviceSwapchainSupport(const vk::PhysicalDevice &physicalDevice, const vk::SurfaceKHR &surface, const Settings &settings);
        static bool CheckPhysicalDeviceExtensionSupport(const vk::PhysicalDevice &physicalDevice, const std::vector<const char *> &requiredExtensions);

        static vk::DebugUtilsMessengerCreateInfoEXT GetDebugUtilsCreateInfo();
        static std::vector<const char *>            GetRequiredInstanceExtensions();

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessengerCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
            VkDebugUtilsMessengerCreateFlagsEXT         type,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void                                       *pUserdata);
    };
}  // namespace vre::Vulkan