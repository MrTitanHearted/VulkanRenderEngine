#pragma once

#include <VREngine/Core.hpp>

namespace vre::Vulkan {
    namespace Image {
        struct Allocation {
            vk::Image     Image;
            vk::Extent3D  Extent;
            vk::Format    Format;
            VmaAllocation Allocation;
            VmaAllocator  Allocator;
        };
    }  // namespace Image

    namespace Buffer {
        struct Allocation {
            vk::Buffer           Buffer;
            vk::BufferUsageFlags UsageFlags;
            std::uint64_t        Size;
            VmaAllocation        Allocation;
            VmaAllocator         Allocator;
        };
    }  // namespace Buffer

    namespace Shader {
        struct StageInfo {
            vk::ShaderModule     Module;
            vk::ShaderStageFlags StageFlags;
            std::string          Entry;
        };
    }  // namespace Shader

    namespace DescriptorLayout {
        struct Binding {
            std::uint32_t        Binding;
            vk::DescriptorType   Type;
            vk::ShaderStageFlags StageFlags;
        };
    }  // namespace DescriptorLayout

    namespace DescriptorPool {
        struct PoolSizeRatio {
            vk::DescriptorType Type;
            float              Ratio;
        };
    }  // namespace DescriptorPool

    struct VertexLayout {
        vk::VertexInputBindingDescription                Binding;
        std::vector<vk::VertexInputAttributeDescription> Attributes;
    };
}  // namespace vre::Vulkan