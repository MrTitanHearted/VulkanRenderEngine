#pragma once

#include <VREngine/Core.hpp>

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
    }  // namespace CommandBuffer
}  // namespace vre::Vulkan