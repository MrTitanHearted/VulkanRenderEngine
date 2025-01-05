#pragma once

#include <VREngine/Vulkan/Types.hpp>

namespace vre::Vulkan::Buffer {
    vk::BufferCreateInfo GetCreateInfo(std::uint64_t size, vk::BufferUsageFlags usageFlags);

    Allocation Allocate(
        VmaAllocationCreateFlags vmaFlags,
        VmaMemoryUsage           memoryUsage,
        std::uint64_t            size,
        vk::BufferUsageFlags     usageFlags,
        const VmaAllocator      &allocator);
    Allocation Allocate(
        VmaMemoryUsage       memoryUsage,
        std::uint64_t        size,
        vk::BufferUsageFlags usageFlags,
        const VmaAllocator  &allocator);
    Allocation Allocate(
        VmaAllocationCreateFlags vmaFlags,
        std::uint64_t            size,
        vk::BufferUsageFlags     usageFlags,
        const VmaAllocator      &allocator);
    Allocation Allocate(
        std::uint64_t        size,
        vk::BufferUsageFlags usageFlags,
        const VmaAllocator  &allocator);
    Allocation AllocateMapped(
        VmaMemoryUsage       memoryUsage,
        std::uint64_t        size,
        vk::BufferUsageFlags usageFlags,
        const VmaAllocator  &allocator);
    Allocation AllocateMapped(
        std::uint64_t        size,
        vk::BufferUsageFlags usageFlags,
        const VmaAllocator  &allocator);

    void Release(const Allocation &buffer);
}  // namespace vre::Vulkan::Buffer