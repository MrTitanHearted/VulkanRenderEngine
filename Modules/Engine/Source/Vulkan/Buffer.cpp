#include <VREngine/Vulkan/Buffer.hpp>

namespace vre::Vulkan::Buffer {
    vk::BufferCreateInfo GetCreateInfo(std::uint64_t size, vk::BufferUsageFlags usageFlags) {
        return vk::BufferCreateInfo{
            {},
            size,
            usageFlags,
        };
    }

    Allocation Allocate(
        VmaAllocationCreateFlags vmaFlags,
        VmaMemoryUsage           memoryUsage,
        std::uint64_t            size,
        vk::BufferUsageFlags     usageFlags,
        const VmaAllocator      &allocator) {
        vk::BufferCreateInfo bufferInfo{
            {},
            size,
            usageFlags,
        };
        VmaAllocationCreateInfo allocationInfo{
            .flags = vmaFlags,
            .usage = memoryUsage,
        };

        VkBuffer      cBuffer{VK_NULL_HANDLE};
        VmaAllocation allocation{VK_NULL_HANDLE};

        VRE_VK_CHECK(vmaCreateBuffer(
            allocator,
            &(const VkBufferCreateInfo &)bufferInfo,
            &allocationInfo,
            &cBuffer,
            &allocation,
            nullptr));

        return Allocation{
            .Buffer     = cBuffer,
            .UsageFlags = usageFlags,
            .Size       = size,
            .Allocation = allocation,
            .Allocator  = allocator,
        };
    }

    Allocation Allocate(
        VmaMemoryUsage       memoryUsage,
        std::uint64_t        size,
        vk::BufferUsageFlags usageFlags,
        const VmaAllocator  &allocator) {
        vk::BufferCreateInfo bufferInfo{
            {},
            size,
            usageFlags,
        };
        VmaAllocationCreateInfo allocationInfo{
            .usage = memoryUsage,
        };

        VkBuffer      cBuffer{VK_NULL_HANDLE};
        VmaAllocation allocation{VK_NULL_HANDLE};

        DVRE_VK_CHECK(vmaCreateBuffer(
            allocator,
            &(const VkBufferCreateInfo &)bufferInfo,
            &allocationInfo,
            &cBuffer,
            &allocation,
            nullptr));

        return Allocation{
            .Buffer     = cBuffer,
            .UsageFlags = usageFlags,
            .Size       = size,
            .Allocation = allocation,
            .Allocator  = allocator,
        };
    }

    Allocation Allocate(
        VmaAllocationCreateFlags vmaFlags,
        std::uint64_t            size,
        vk::BufferUsageFlags     usageFlags,
        const VmaAllocator      &allocator) {
        vk::BufferCreateInfo bufferInfo{
            {},
            size,
            usageFlags,
        };
        VmaAllocationCreateInfo allocationInfo{
            .flags = vmaFlags,
            .usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
        };

        VkBuffer      cBuffer{VK_NULL_HANDLE};
        VmaAllocation allocation{VK_NULL_HANDLE};

        DVRE_VK_CHECK(vmaCreateBuffer(
            allocator,
            &(const VkBufferCreateInfo &)bufferInfo,
            &allocationInfo,
            &cBuffer,
            &allocation,
            nullptr));

        return Allocation{
            .Buffer     = cBuffer,
            .UsageFlags = usageFlags,
            .Size       = size,
            .Allocation = allocation,
            .Allocator  = allocator,
        };
    }

    Allocation Allocate(std::uint64_t size, vk::BufferUsageFlags usageFlags, const VmaAllocator &allocator) {
        vk::BufferCreateInfo bufferInfo{
            {},
            size,
            usageFlags,
        };
        VmaAllocationCreateInfo allocationInfo{
            .usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
        };

        VkBuffer      cBuffer{VK_NULL_HANDLE};
        VmaAllocation allocation{VK_NULL_HANDLE};

        DVRE_VK_CHECK(vmaCreateBuffer(
            allocator,
            &(const VkBufferCreateInfo &)bufferInfo,
            &allocationInfo,
            &cBuffer,
            &allocation,
            nullptr));

        return Allocation{
            .Buffer     = cBuffer,
            .UsageFlags = usageFlags,
            .Size       = size,
            .Allocation = allocation,
            .Allocator  = allocator,
        };
    }

    Allocation AllocateMapped(
        VmaMemoryUsage       memoryUsage,
        std::uint64_t        size,
        vk::BufferUsageFlags usageFlags,
        const VmaAllocator  &allocator) {
        vk::BufferCreateInfo bufferInfo{
            {},
            size,
            usageFlags,
        };
        VmaAllocationCreateInfo allocationInfo{
            .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = memoryUsage,
        };

        VkBuffer      cBuffer{VK_NULL_HANDLE};
        VmaAllocation allocation{VK_NULL_HANDLE};

        DVRE_VK_CHECK(vmaCreateBuffer(
            allocator,
            &(const VkBufferCreateInfo &)bufferInfo,
            &allocationInfo,
            &cBuffer,
            &allocation,
            nullptr));

        return Allocation{
            .Buffer     = cBuffer,
            .UsageFlags = usageFlags,
            .Size       = size,
            .Allocation = allocation,
            .Allocator  = allocator,
        };
    }

    Allocation AllocateMapped(
        std::uint64_t        size,
        vk::BufferUsageFlags usageFlags,
        const VmaAllocator  &allocator) {
        vk::BufferCreateInfo bufferInfo{
            {},
            size,
            usageFlags,
        };
        VmaAllocationCreateInfo allocationInfo{
            .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
        };

        VkBuffer      cBuffer{VK_NULL_HANDLE};
        VmaAllocation allocation{VK_NULL_HANDLE};

        DVRE_VK_CHECK(vmaCreateBuffer(
            allocator,
            &(const VkBufferCreateInfo &)bufferInfo,
            &allocationInfo,
            &cBuffer,
            &allocation,
            nullptr));

        return Allocation{
            .Buffer     = cBuffer,
            .UsageFlags = usageFlags,
            .Size       = size,
            .Allocation = allocation,
            .Allocator  = allocator,
        };
    }

    void Release(const Allocation &buffer) {
        vmaDestroyBuffer(buffer.Allocator, buffer.Buffer, buffer.Allocation);
    }
}  // namespace vre::Vulkan::Buffer