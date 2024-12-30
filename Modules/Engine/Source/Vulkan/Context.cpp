#include <VREngine/Vulkan/Context.hpp>

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, VkDebugUtilsMessengerEXT *pDebugMessenger) {
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func == nullptr)
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    return func(instance, pCreateInfo, nullptr, pDebugMessenger);
}

VkResult DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger) {
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func == nullptr)
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    func(instance, debugMessenger, nullptr);
    return VK_SUCCESS;
}

namespace vre::Vulkan {
    vk::Instance               Context::g_Instance{VK_NULL_HANDLE};
    vk::DebugUtilsMessengerEXT Context::g_DebugMessenger{VK_NULL_HANDLE};
    vk::SurfaceKHR             Context::g_Surface{VK_NULL_HANDLE};
    vk::PhysicalDevice         Context::g_PhysicalDevice{VK_NULL_HANDLE};
    vk::Device                 Context::g_Device{VK_NULL_HANDLE};
    std::uint32_t              Context::g_QueueFamilyIndex{0u};
    std::uint32_t              Context::g_QueueFamilyQueueCount{0u};
    vk::Queue                  Context::g_GraphicsQueue{VK_NULL_HANDLE};
    vk::Queue                  Context::g_ComputeQueue{VK_NULL_HANDLE};
    vk::Queue                  Context::g_PresentQueue{VK_NULL_HANDLE};
    vk::SwapchainKHR           Context::g_Swapchain{VK_NULL_HANDLE};
    vk::SurfaceFormatKHR       Context::g_SwapchainFormat{};
    vk::Extent2D               Context::g_SwapchainExtent{};
    vk::PresentModeKHR         Context::g_SwapchainPresentMode{};
    vk::ImageUsageFlags        Context::g_SwapchainUsageFlags{0u};
    std::uint32_t              Context::g_SwapchainImageCount{0u};
    std::vector<vk::Image>     Context::g_SwapchainImages{};
    std::vector<vk::ImageView> Context::g_SwapchainImageViews{};
    VmaAllocator               Context::g_VmaAllocator{VK_NULL_HANDLE};
    bool                       Context::g_IsInitialized{false};
    Context                    Context::g_State{};

    void Context::Initialize(const Settings &settings) {
        DVRE_ASSERT(Window::IsInitialized(), "vre::Window must be initialized");
        DVRE_ASSERT(!g_IsInitialized, "vre::Vulkan::Context must be shut down before initializing");
        DLOG_INFO("Initializing vre::Vulkan::Context");

        std::vector<const char *> instanceExtensions = GetRequiredInstanceExtensions();
        std::vector<const char *> instanceLayers{
#if defined(VRE_BUILD_TYPE_DEBUG)
            "VK_LAYER_KHRONOS_validation",
#endif
        };
        std::vector<const char *> deviceExtensions{
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
            VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
            VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
            VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
            VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,
            VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME,
        };

        VRE_ASSERT(CheckInstanceLayerSupport(instanceLayers), "This Vulkan Instance does not support required Layers");

        vk::ApplicationInfo appInfo{
            "Vulkan Render Engine",
            VK_MAKE_VERSION(1u, 0u, 0u),
            "Vulkan Render Engine",
            VK_MAKE_VERSION(1u, 0u, 0u),
            VK_API_VERSION_1_3,
        };
        vk::InstanceCreateInfo instanceInfo{
            // vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR,
            {},
            &appInfo,
            instanceLayers,
            instanceExtensions,
        };

        {
            auto [result, instance] = vk::createInstance(instanceInfo);
            VRE_VK_CHECK(result);
            g_Instance = instance;
        }

#if defined(VRE_BUILD_TYPE_DEBUG)
        vk::DebugUtilsMessengerCreateInfoEXT debugMessengerInfo = GetDebugUtilsCreateInfo();
        instanceInfo.setPNext(&debugMessengerInfo);
#endif

#if defined(VRE_BUILD_TYPE_DEBUG)
        VkDebugUtilsMessengerEXT cDebugMessenger{VK_NULL_HANDLE};
        VRE_VK_CHECK(CreateDebugUtilsMessengerEXT(g_Instance, &(const VkDebugUtilsMessengerCreateInfoEXT &)debugMessengerInfo, &cDebugMessenger));
        g_DebugMessenger = cDebugMessenger;
#endif

        VkSurfaceKHR cSurface{VK_NULL_HANDLE};
        VRE_VK_CHECK(glfwCreateWindowSurface(g_Instance, Window::GetGlfwWindow(), nullptr, &cSurface));
        g_Surface = cSurface;

        SelectPhysicalDevice(deviceExtensions, settings);
        SelectQueueFamilyIndex();
        SelectSwapchainImageCount(settings);
        SelectSwapchainFormat(settings);
        SelectSwapchainPresentMode(settings);
        g_SwapchainUsageFlags =
            vk::ImageUsageFlagBits::eColorAttachment |
            vk::ImageUsageFlagBits::eTransferDst |
            vk::ImageUsageFlagBits::eTransferSrc |
            settings.SurfaceUsageFlags;

        vk::PhysicalDeviceExtendedDynamicState2FeaturesEXT dynamicFeatures2{};
        vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT  dynamicFeatures{};

        vk::PhysicalDeviceVulkan13Features features13{};
        vk::PhysicalDeviceVulkan12Features features12{};
        vk::PhysicalDeviceFeatures2        features{};

        dynamicFeatures2.setExtendedDynamicState2(vk::True);
        dynamicFeatures
            .setExtendedDynamicState(vk::True)
            .setPNext(&dynamicFeatures2);
        features13
            .setDynamicRendering(vk::True)
            .setSynchronization2(vk::True)
            .setPNext(&dynamicFeatures);
        features12
            .setBufferDeviceAddress(vk::True)
            .setDescriptorIndexing(vk::True)
            .setPNext(&features13);
        features.setPNext(&features12);

        std::vector<float> queuePriorities{};
        queuePriorities.reserve(5U);

        for (std::uint32_t i = 0; i < g_QueueFamilyQueueCount; i++) {
            queuePriorities.emplace_back(1.0f - float(i) / 10.0f);
        }

        vk::DeviceQueueCreateInfo queueInfo{
            {},
            g_QueueFamilyIndex,
            queuePriorities,
        };
        vk::DeviceCreateInfo deviceInfo{
            {},
            {queueInfo},
#if defined(VRE_BUILD_TYPE_DEBUG)
            instanceLayers,
#else
            {},
#endif
            deviceExtensions,
            nullptr,
            &features,
        };

        {
            auto [result, device] = g_PhysicalDevice.createDevice(deviceInfo);
            VRE_VK_CHECK(result);
            g_Device = device;
        }

        std::uint32_t queueIndex = 0u;

        g_GraphicsQueue = g_Device.getQueue(g_QueueFamilyIndex, queueIndex);
        if (queueIndex + 1 < g_QueueFamilyQueueCount) queueIndex++;
        g_PresentQueue = g_Device.getQueue(g_QueueFamilyIndex, queueIndex);
        if (queueIndex + 1 < g_QueueFamilyQueueCount) queueIndex++;
        g_ComputeQueue = g_Device.getQueue(g_QueueFamilyIndex, queueIndex);

        CreateSwapchain();

        VmaAllocatorCreateInfo vmaInfo{
            .flags            = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
            .physicalDevice   = g_PhysicalDevice,
            .device           = g_Device,
            .instance         = g_Instance,
            .vulkanApiVersion = VK_API_VERSION_1_3,
        };
        VRE_VK_CHECK(vmaCreateAllocator(&vmaInfo, &g_VmaAllocator));

        g_IsInitialized = true;
    }

    void Context::Shutdown() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized before shutting down");
        DLOG_INFO("Shutting vre::Vulkan::Context down");

        VRE_VK_CHECK(g_Device.waitIdle());
        vmaDestroyAllocator(g_VmaAllocator);
        ReleaseSwapchain();
        g_Device.destroy();
        g_Instance.destroy(g_Surface);
#if defined(VRE_BUILD_TYPE_DEBUG)
        VRE_VK_CHECK(DestroyDebugUtilsMessengerEXT(g_Instance, g_DebugMessenger));
#endif
        g_Instance.destroy();
        g_IsInitialized = false;
    }

    bool Context::IsInitialized() {
        return g_IsInitialized;
    }

    void Context::Resize() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        auto [width, height] = Window::GetSize();
        DVRE_ASSERT(width != 0 && height != 0, "vre::Vulkan::Context cannot resize with width or height or both being 0. You should deal with it when vre::Window resizes to 0");
        if (g_SwapchainExtent.width == width && g_SwapchainExtent.height == height) return;
        VRE_VK_CHECK(g_Device.waitIdle());
        CreateSwapchain();
    }

    vk::Instance Context::GetInstance() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_Instance;
    }

    vk::DebugUtilsMessengerEXT Context::GetDebugUtilsMessenger() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_DebugMessenger;
    }

    vk::SurfaceKHR Context::GetSurface() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_Surface;
    }

    vk::PhysicalDevice Context::GetPhysicalDevice() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_PhysicalDevice;
    }

    vk::Device Context::GetDevice() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_Device;
    }

    std::uint32_t Context::GetQueueFamilyIndex() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_QueueFamilyIndex;
    }

    vk::Queue Context::GetGraphicsQueue() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_GraphicsQueue;
    }

    vk::Queue Context::GetComputeQueue() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_ComputeQueue;
    }

    vk::Queue Context::GetPresentQueue() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_PresentQueue;
    }

    vk::SwapchainKHR Context::GetSwapchain() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_Swapchain;
    }

    vk::Format Context::GetSwapchainFormat() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_SwapchainFormat.format;
    }

    vk::Extent2D Context::GetSwapchainExtent() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_SwapchainExtent;
    }

    vk::PresentModeKHR Context::GetSwapchainPresentMode() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_SwapchainPresentMode;
    }

    vk::ImageUsageFlags Context::GetSwapchainUsageFlags() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_SwapchainUsageFlags;
    }

    std::vector<vk::Image> Context::GetSwapchainImages() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_SwapchainImages;
    }

    std::vector<vk::ImageView> Context::GetSwapchainImageViews() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_SwapchainImageViews;
    }

    VmaAllocator Context::GetVmaAllocator() {
        DVRE_ASSERT(g_IsInitialized, "vre::Vulkan::Context must be initialized");
        return g_VmaAllocator;
    }

    void Context::SelectPhysicalDevice(const std::vector<const char *> &requiredExtensions, const Settings &settings) {
        auto [result, physicalDevices] = g_Instance.enumeratePhysicalDevices();
        VRE_VK_CHECK(result);
        DVRE_ASSERT(physicalDevices.size() > 0, "You need a GPU that supports Vulkan 1.3");

        for (const vk::PhysicalDevice &physicalDevice : physicalDevices) {
            if (CheckPhysicalDeviceFeatureSupport(physicalDevice) &&
                CheckPhysicalDeviceSwapchainSupport(physicalDevice, g_Surface, settings) &&
                CheckPhysicalDeviceExtensionSupport(physicalDevice, requiredExtensions)) {
                vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();

                std::uint32_t apiVersion    = properties.apiVersion;
                std::uint32_t driverVersion = properties.driverVersion;

                std::uint32_t apiVersionMajor = VK_VERSION_MAJOR(apiVersion);
                std::uint32_t apiVersionMinor = VK_VERSION_MINOR(apiVersion);
                std::uint32_t apiVersionPatch = VK_VERSION_PATCH(apiVersion);

                DLOG_DEBUG("Selecting GPU:");
                DLOG_DEBUG("\tAPI Version: {}.{}.{}", apiVersionMajor, apiVersionMinor, apiVersionPatch);
                DLOG_DEBUG("\tDevice ID: {}", properties.deviceID);
                DLOG_DEBUG("\tVendor ID: {}", properties.vendorID);
                DLOG_DEBUG("\tDevice Name: {}", properties.deviceName.data());
                DLOG_DEBUG("\tDevice Type: {}", vk::to_string(properties.deviceType));
                DLOG_DEBUG("\tDriver Version: {}", driverVersion);

                g_PhysicalDevice = physicalDevice;
                return;
            }
        }

        VRE_ASSERT(false, "There are no available GPUs that supports Vulkan 1.3 or VulkanRenderEngine features");
    }

    void Context::SelectQueueFamilyIndex() {
        std::vector<vk::QueueFamilyProperties> queueFamilies = g_PhysicalDevice.getQueueFamilyProperties();
        DVRE_ASSERT(!queueFamilies.empty(), "There are no Vulkan Queue Families available!!!");

        std::uint32_t index = 0u;
        for (const vk::QueueFamilyProperties &queueFamily : queueFamilies) {
            auto [result, presentationSupport] = g_PhysicalDevice.getSurfaceSupportKHR(index, g_Surface);
            VRE_VK_CHECK(result);
            if (presentationSupport == vk::True &&
                queueFamily.queueCount >= 3u &&
                std::uint32_t(queueFamily.queueFlags &
                              (vk::QueueFlagBits::eGraphics |
                               vk::QueueFlagBits::eCompute)) > 0) {
                g_QueueFamilyIndex      = index;
                g_QueueFamilyQueueCount = 3u;
                return;
            }
            index++;
        }

        g_QueueFamilyIndex      = 0;
        g_QueueFamilyQueueCount = queueFamilies[0].queueCount;
    }

    void Context::SelectSwapchainImageCount(const Settings &settings) {
        auto [result, surfaceCapabilities] = g_PhysicalDevice.getSurfaceCapabilitiesKHR(g_Surface);
        VRE_VK_CHECK(result);

        std::uint32_t imageCount = settings.PreferredImageCount;
        if (surfaceCapabilities.minImageCount > 0 && imageCount < surfaceCapabilities.minImageCount + 1) {
            imageCount = surfaceCapabilities.minImageCount + 1;
        }
        if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount) {
            imageCount = surfaceCapabilities.maxImageCount;
        }

        g_SwapchainImageCount = imageCount;
    }

    void Context::SelectSwapchainFormat(const Settings &settings) {
        auto [result, surfaceFormats] = g_PhysicalDevice.getSurfaceFormatsKHR(g_Surface);
        VRE_VK_CHECK(result);

        for (const vk::Format &preferredFormat : settings.PreferredSurfaceFormats)
            for (const vk::SurfaceFormatKHR &format : surfaceFormats)
                if (format.format == preferredFormat &&
                    format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                    g_SwapchainFormat = format;
                    return;
                }

        g_SwapchainFormat = surfaceFormats[0];
    }

    void Context::SelectSwapchainPresentMode(const Settings &settings) {
        auto [result, presentModes] = g_PhysicalDevice.getSurfacePresentModesKHR(g_Surface);
        VRE_VK_CHECK(result);

        for (const vk::PresentModeKHR &preferredMode : settings.PreferredPresentModes)
            for (const vk::PresentModeKHR &mode : presentModes)
                if (mode == preferredMode) {
                    g_SwapchainPresentMode = mode;
                    return;
                }

        g_SwapchainPresentMode = vk::PresentModeKHR::eFifo;
    }

    void Context::CreateSwapchain() {
        auto [result, surfaceCapabilities] = g_PhysicalDevice.getSurfaceCapabilitiesKHR(g_Surface);
        VRE_VK_CHECK(result);

        auto [width, height]     = Window::GetSize();
        g_SwapchainExtent.width  = width;
        g_SwapchainExtent.height = height;

        vk::SwapchainKHR oldSwapchain = g_Swapchain;

        vk::SwapchainCreateInfoKHR swapchainInfo{
            {},
            g_Surface,
            g_SwapchainImageCount,
            g_SwapchainFormat.format,
            g_SwapchainFormat.colorSpace,
            g_SwapchainExtent,
            1,
            g_SwapchainUsageFlags,
            vk::SharingMode::eExclusive,
            {g_QueueFamilyIndex},
            surfaceCapabilities.currentTransform,
            vk::CompositeAlphaFlagBitsKHR::eOpaque,
            g_SwapchainPresentMode,
            vk::True,
            oldSwapchain,
        };

        for (const vk::ImageView &view : g_SwapchainImageViews)
            g_Device.destroy(view);

        g_SwapchainImageViews.clear();
        g_SwapchainImages.clear();

        {
            auto [result, swapchain] = g_Device.createSwapchainKHR(swapchainInfo);
            VRE_VK_CHECK(result);
            g_Swapchain = swapchain;
        }
        {
            auto [result, swapchainImages] = g_Device.getSwapchainImagesKHR(g_Swapchain);
            VRE_VK_CHECK(result);
            g_SwapchainImages = swapchainImages;
        }
        g_SwapchainImageViews.reserve(g_SwapchainImages.size());
        for (const vk::Image &image : g_SwapchainImages) {
            auto [result, view] = g_Device.createImageView(
                vk::ImageViewCreateInfo{}
                    .setViewType(vk::ImageViewType::e2D)
                    .setImage(image)
                    .setFormat(g_SwapchainFormat.format)
                    .setComponents(vk::ComponentMapping{}
                                       .setR(vk::ComponentSwizzle::eIdentity)
                                       .setG(vk::ComponentSwizzle::eIdentity)
                                       .setB(vk::ComponentSwizzle::eIdentity)
                                       .setA(vk::ComponentSwizzle::eIdentity))
                    .setSubresourceRange(
                        vk::ImageSubresourceRange{}
                            .setAspectMask(vk::ImageAspectFlagBits::eColor)
                            .setBaseMipLevel(0)
                            .setLevelCount(1)
                            .setBaseArrayLayer(0)
                            .setLayerCount(1)));
            VRE_VK_CHECK(result);
            g_SwapchainImageViews.emplace_back(view);
        }
        if (!oldSwapchain) {
            g_Device.destroy(oldSwapchain);
        }
    }

    void Context::ReleaseSwapchain() {
        for (const vk::ImageView &view : g_SwapchainImageViews)
            g_Device.destroy(view);
        g_SwapchainImages.clear();
        g_SwapchainImageViews.clear();
        g_Device.destroy(g_Swapchain);
    }

    bool Context::CheckInstanceLayerSupport(const std::vector<const char *> &requiredLayers) {
        auto [result, availableLayers] = vk::enumerateInstanceLayerProperties();
        VRE_VK_CHECK(result);

        for (const char *const &layerName : requiredLayers) {
            bool layerFound = false;

            for (const vk::LayerProperties &layerProperty : availableLayers) {
                if (strcmp(layerName, layerProperty.layerName)) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    bool Context::CheckPhysicalDeviceFeatureSupport(const vk::PhysicalDevice &physicalDevice) {
        vk::PhysicalDeviceExtendedDynamicState2FeaturesEXT dynamicFeatures2{};
        vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT  dynamicFeatures{};

        vk::PhysicalDeviceVulkan13Features features13{};
        vk::PhysicalDeviceVulkan12Features features12{};
        vk::PhysicalDeviceFeatures2        features2{};

        dynamicFeatures.setPNext(&dynamicFeatures2);
        features13.setPNext(&dynamicFeatures);
        features12.setPNext(&features13);
        features2.setPNext(&features12);

        physicalDevice.getFeatures2(&features2);

        return dynamicFeatures2.extendedDynamicState2 == vk::True &&
               dynamicFeatures.extendedDynamicState == vk::True &&
               features13.dynamicRendering == vk::True &&
               features13.synchronization2 == vk::True &&
               features12.bufferDeviceAddress == vk::True &&
               features12.descriptorIndexing == vk::True;
    }

    bool Context::CheckPhysicalDeviceSwapchainSupport(const vk::PhysicalDevice &physicalDevice, const vk::SurfaceKHR &surface, const Settings &settings) {
        auto [result, surfaceCapabilities] = physicalDevice.getSurfaceCapabilitiesKHR(surface);
        VRE_VK_CHECK(result);

        vk::ImageUsageFlags usages = surfaceCapabilities.supportedUsageFlags;
        vk::ImageUsageFlags requiredUsages =
            vk::ImageUsageFlagBits::eColorAttachment |
            vk::ImageUsageFlagBits::eTransferDst |
            vk::ImageUsageFlagBits::eTransferSrc |
            settings.SurfaceUsageFlags;
        if (!(std::uint32_t(usages & requiredUsages) > 0u))
            return false;
        return true;
    }

    bool Context::CheckPhysicalDeviceExtensionSupport(const vk::PhysicalDevice &physicalDevice, const std::vector<const char *> &requiredExtensions) {
        auto [result, availableExtensions] = physicalDevice.enumerateDeviceExtensionProperties();
        VRE_VK_CHECK(result);

        std::set<std::string> extensions{requiredExtensions.begin(), requiredExtensions.end()};
        for (const vk::ExtensionProperties &extension : availableExtensions) {
            extensions.erase(extension.extensionName.data());
            if (extensions.empty()) {
                return true;
            }
        }
        return false;
    }

    vk::DebugUtilsMessengerCreateInfoEXT Context::GetDebugUtilsCreateInfo() {
        return vk::DebugUtilsMessengerCreateInfoEXT{}
            .setMessageSeverity(
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
            .setMessageType(
                vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
            .setPfnUserCallback(Context::DebugUtilsMessengerCallback);
    }

    std::vector<const char *> Context::GetRequiredInstanceExtensions() {
        std::uint32_t glfwExtensionCount = 0u;
        const char  **glfwExtensions     = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char *> extensions{glfwExtensions, glfwExtensions + glfwExtensionCount};

#ifdef VRE_BUILD_TYPE_DEBUG
        extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        return extensions;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL Context::DebugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessengerCreateFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserdata) {
        switch (severity) {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
                VRE_ERROR("[Vulkan Validation Layers]: {}", pCallbackData->pMessage);
            } break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
                VRE_WARN("[Vulkan Validation Layers]: {}", pCallbackData->pMessage);
            } break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
                VRE_DEBUG("[Vulkan Validation Layers]: {}", pCallbackData->pMessage);
            } break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
                VRE_INFO("[Vulkan Validation Layers]: {}", pCallbackData->pMessage);
            } break;
            default:
                break;
        }

        return VK_FALSE;
    }

    Context::~Context() {
        VRE_ASSERT(!g_IsInitialized, "vre::Vulkan::Context must be shut down before closing!");
    }
}  // namespace vre::Vulkan