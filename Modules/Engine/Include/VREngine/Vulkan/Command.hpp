#pragma once

#include <VREngine/Core.hpp>
#include <VREngine/Vulkan/Types.hpp>

namespace vre::Vulkan {
    namespace CommandPool {
        vk::CommandPoolCreateInfo GetCreateInfo(
            vk::CommandPoolCreateFlags flags,
            std::uint32_t              queueFamilyIndex);
        vk::CommandPoolCreateInfo GetCreateInfo(std::uint32_t queueFamilyIndex);
        vk::CommandPoolCreateInfo GetTransientCreateInfo(std::uint32_t queueFamilyIndex);
        vk::CommandPoolCreateInfo GetResetCommandBufferCreateInfo(std::uint32_t queueFamilyIndex);

        vk::CommandPool Create(
            vk::CommandPoolCreateFlags flags,
            std::uint32_t              queueFamilyIndex,
            const vk::Device          &device);
        vk::CommandPool Create(std::uint32_t queueFamilyIndex, const vk::Device &device);
        vk::CommandPool CreateTransient(std::uint32_t queueFamilyIndex, const vk::Device &device);
        vk::CommandPool CreateResetCommandBuffer(
            std::uint32_t     queueFamilyIndex,
            const vk::Device &device);
    }  // namespace CommandPool

    namespace CommandBuffer {
        vk::CommandBufferAllocateInfo GetPrimaryAllocateInfo(
            std::uint32_t          count,
            const vk::CommandPool &pool);
        vk::CommandBufferAllocateInfo GetSecondaryAllocateInfo(
            std::uint32_t          count,
            const vk::CommandPool &pool);
        vk::CommandBufferAllocateInfo GetPrimaryAllocateInfo(const vk::CommandPool &pool);
        vk::CommandBufferAllocateInfo GetSecondaryAllocateInfo(const vk::CommandPool &pool);

        std::vector<vk::CommandBuffer> AllocatePrimary(
            std::uint32_t          count,
            const vk::CommandPool &pool,
            const vk::Device      &device);
        std::vector<vk::CommandBuffer> AllocateSecondary(
            std::uint32_t          count,
            const vk::CommandPool &pool,
            const vk::Device      &device);
        vk::CommandBuffer AllocatePrimary(const vk::CommandPool &pool, const vk::Device &device);
        vk::CommandBuffer AllocateSecondary(const vk::CommandPool &pool, const vk::Device &device);

        vk::CommandBufferBeginInfo GetBeginInfo(vk::CommandBufferUsageFlags flags);
        vk::CommandBufferBeginInfo GetBeginInfo();
        vk::CommandBufferBeginInfo GetOneTimeSubmitBeginInfo();

        vk::CommandBufferSubmitInfo GetSubmitInfo(const vk::CommandBuffer &buffer);

        void Begin(vk::CommandBufferUsageFlags flags, const vk::CommandBuffer &buffer);
        void Begin(const vk::CommandBuffer &buffer);
        void BeginOneTimeSubmit(const vk::CommandBuffer &buffer);

        void End(const vk::CommandBuffer &buffer);

        void CopyImageToImage(
            const vk::CommandBuffer &buffer,
            const Image::Allocation &source,
            const Image::Allocation &destination);
        void CopyImageToImage(
            const vk::CommandBuffer &buffer,
            const vk::Image         &source,
            const vk::Image         &destination,
            const vk::Extent3D      &size);
        void CopyImageToImage(
            const vk::CommandBuffer &buffer,
            const vk::Image         &source,
            const vk::Image         &destination,
            const vk::Extent2D      &size);
        void CopyImageToImage(
            const vk::CommandBuffer &buffer,
            const vk::Image         &source,
            const vk::Image         &destination,
            const vk::Extent3D      &sourceExtent,
            const vk::Extent3D      &destinationExtent);
        void CopyImageToImage(
            const vk::CommandBuffer &buffer,
            const vk::Image         &source,
            const vk::Image         &destination,
            const vk::Extent2D      &sourceExtent,
            const vk::Extent2D      &destinationExtent);

        void CopyBufferToBuffer(
            const vk::CommandBuffer  &buffer,
            const Buffer::Allocation &source,
            const Buffer::Allocation &destination,
            std::uint64_t             sourceOffset,
            std::uint64_t             destinationOffset,
            std::uint64_t             size);
        void CopyBufferToBuffer(
            const vk::CommandBuffer  &buffer,
            const Buffer::Allocation &source,
            const Buffer::Allocation &destination,
            std::uint64_t             size);
        void CopyBufferToBuffer(
            const vk::CommandBuffer &buffer,
            const vk::Buffer        &source,
            const vk::Buffer        &destination,
            std::uint64_t            sourceOffset,
            std::uint64_t            destinationOffset,
            std::uint64_t            size);
        void CopyBufferToBuffer(
            const vk::CommandBuffer &buffer,
            const vk::Buffer        &source,
            const vk::Buffer        &destination,
            std::uint64_t            size);

        void CopyBufferToImage(
            const vk::CommandBuffer  &buffer,
            const Buffer::Allocation &source,
            const Image::Allocation  &destination);
        void CopyBufferToImage(
            const vk::CommandBuffer  &buffer,
            const Buffer::Allocation &source,
            const vk::Image          &destination,
            const vk::Extent3D       &size);
        void CopyBufferToImage(
            const vk::CommandBuffer &buffer,
            const vk::Buffer        &source,
            const Image::Allocation &destination);
        void CopyBufferToImage(
            const vk::CommandBuffer &buffer,
            const vk::Buffer        &source,
            const vk::Image         &destination,
            const vk::Extent3D      &size);

        void CopyImageToBuffer(
            const vk::CommandBuffer  &buffer,
            const Image::Allocation  &source,
            const Buffer::Allocation &destination);
        void CopyImageToBuffer(
            const vk::CommandBuffer &buffer,
            const Image::Allocation &source,
            const vk::Buffer        &destination);
        void CopyImageToBuffer(
            const vk::CommandBuffer  &buffer,
            const vk::Image          &source,
            const Buffer::Allocation &destination,
            const vk::Extent3D       &size);
        void CopyImageToBuffer(
            const vk::CommandBuffer &buffer,
            const vk::Image         &source,
            const vk::Buffer        &destination,
            const vk::Extent3D      &size);

    }  // namespace CommandBuffer
}  // namespace vre::Vulkan