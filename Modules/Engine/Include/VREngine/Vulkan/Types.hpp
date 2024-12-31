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
            std::uint32_t        Size;
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

    struct VertexLayout {
        vk::VertexInputBindingDescription                Binding;
        std::vector<vk::VertexInputAttributeDescription> Attributes;
    };
}  // namespace vre::Vulkan