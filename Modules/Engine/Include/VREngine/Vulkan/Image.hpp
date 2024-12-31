#pragma once

#include <VREngine/Core.hpp>
#include <VREngine/Vulkan/Types.hpp>

namespace vre::Vulkan::Image {
    void TransitionLayout(
        const vk::CommandBuffer &commandBuffer,
        const Allocation        &image,
        vk::ImageLayout          oldLayout,
        vk::ImageLayout          newLayout);
    void TransitionLayout(
        const vk::CommandBuffer &commandBuffer,
        const Allocation        &image,
        vk::ImageLayout          newLayout);
    void TransitionLayout(
        const vk::CommandBuffer &commandBuffer,
        const vk::Image         &image,
        vk::ImageLayout          oldLayout,
        vk::ImageLayout          newLayout);
    void TransitionLayout(
        const vk::CommandBuffer &commandBuffer,
        const vk::Image         &image,
        vk::ImageLayout          newLayout);

    void Release(const Allocation &image);

    vk::ImageSubresourceRange GetSubresourceRange(vk::ImageAspectFlags aspectFlags);
    vk::ImageSubresourceRange GetColorRange();
    vk::ImageSubresourceRange GetDepthRange();

    vk::ImageCreateInfo GetCreateInfo(
        vk::Format          format,
        vk::ImageUsageFlags usageFlags,
        const vk::Extent3D &extent);
    vk::ImageCreateInfo GetCreateInfo(
        vk::Format          format,
        vk::ImageUsageFlags usageFlags,
        const vk::Extent2D &extent);

    vk::ImageViewCreateInfo GetViewCreateInfo(
        const Allocation    &image,
        vk::ImageAspectFlags aspectFlags);
    vk::ImageViewCreateInfo GetColorViewCreateInfo(const Allocation &image);
    vk::ImageViewCreateInfo GetDepthViewCreateInfo(const Allocation &image);
    vk::ImageViewCreateInfo GetViewCreateInfo3D(
        const vk::Image     &image,
        vk::Format           format,
        vk::ImageAspectFlags aspectFlags);
    vk::ImageViewCreateInfo GetColorViewCreateInfo3D(const vk::Image &image, vk::Format format);
    vk::ImageViewCreateInfo GetDepthViewCreateInfo3D(const vk::Image &image, vk::Format format);
    vk::ImageViewCreateInfo GetViewCreateInfo2D(
        const vk::Image     &image,
        vk::Format           format,
        vk::ImageAspectFlags aspectFlags);
    vk::ImageViewCreateInfo GetColorViewCreateInfo2D(const vk::Image &image, vk::Format format);
    vk::ImageViewCreateInfo GetDepthViewCreateInfo2D(const vk::Image &image, vk::Format format);

    Allocation Allocate(
        VmaAllocationCreateFlags vmaFlags,
        vk::MemoryPropertyFlags  memoryFlags,
        vk::Format               format,
        vk::ImageUsageFlags      usageFlags,
        const vk::Extent3D      &extent,
        const VmaAllocator      &allocator);
    Allocation Allocate(
        VmaAllocationCreateFlags vmaFlags,
        vk::MemoryPropertyFlags  memoryFlags,
        vk::Format               format,
        vk::ImageUsageFlags      usageFlags,
        const vk::Extent2D      &extent,
        const VmaAllocator      &allocator);
    Allocation Allocate(
        vk::MemoryPropertyFlags memoryFlags,
        vk::Format              format,
        vk::ImageUsageFlags     usageFlags,
        const vk::Extent3D     &extent,
        const VmaAllocator     &allocator);
    Allocation Allocate(
        vk::MemoryPropertyFlags memoryFlags,
        vk::Format              format,
        vk::ImageUsageFlags     usageFlags,
        const vk::Extent2D     &extent,
        const VmaAllocator     &allocator);
}  // namespace vre::Vulkan::Image