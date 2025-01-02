#include <VREngine/Vulkan/Command.hpp>

namespace vre::Vulkan {
    namespace CommandPool {
        vk::CommandPoolCreateInfo GetCreateInfo(
            vk::CommandPoolCreateFlags flags,
            std::uint32_t              queueFamilyIndex) {
            return vk::CommandPoolCreateInfo{flags, queueFamilyIndex};
        }

        vk::CommandPoolCreateInfo GetCreateInfo(std::uint32_t queueFamilyIndex) {
            return vk::CommandPoolCreateInfo{{}, queueFamilyIndex};
        }

        vk::CommandPoolCreateInfo GetTransientCreateInfo(std::uint32_t queueFamilyIndex) {
            return vk::CommandPoolCreateInfo{
                vk::CommandPoolCreateFlagBits::eTransient,
                queueFamilyIndex,
            };
        }

        vk::CommandPoolCreateInfo GetResetCommandBufferCreateInfo(std::uint32_t queueFamilyIndex) {
            return vk::CommandPoolCreateInfo{
                vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                queueFamilyIndex,
            };
        }

        vk::CommandPool Create(
            vk::CommandPoolCreateFlags flags,
            std::uint32_t              queueFamilyIndex,
            const vk::Device          &device) {
            auto [result, pool] = device.createCommandPool(vk::CommandPoolCreateInfo{
                flags,
                queueFamilyIndex,
            });
            VRE_VK_CHECK(result);
            return pool;
        }

        vk::CommandPool Create(std::uint32_t queueFamilyIndex, const vk::Device &device) {
            auto [result, pool] = device.createCommandPool(vk::CommandPoolCreateInfo{
                {},
                queueFamilyIndex,
            });
            VRE_VK_CHECK(result);
            return pool;
        }

        vk::CommandPool CreateTransient(std::uint32_t queueFamilyIndex, const vk::Device &device) {
            auto [result, pool] = device.createCommandPool(vk::CommandPoolCreateInfo{
                vk::CommandPoolCreateFlagBits::eTransient,
                queueFamilyIndex,
            });
            VRE_VK_CHECK(result);
            return pool;
        }

        vk::CommandPool CreateResetCommandBuffer(
            std::uint32_t     queueFamilyIndex,
            const vk::Device &device) {
            auto [result, pool] = device.createCommandPool(vk::CommandPoolCreateInfo{
                vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                queueFamilyIndex,
            });
            VRE_VK_CHECK(result);
            return pool;
        }
    }  // namespace CommandPool

    namespace CommandBuffer {
        vk::CommandBufferAllocateInfo GetPrimaryAllocateInfo(
            std::uint32_t          count,
            const vk::CommandPool &pool) {
            return vk::CommandBufferAllocateInfo{
                pool,
                vk::CommandBufferLevel::ePrimary,
                count,
            };
        }

        vk::CommandBufferAllocateInfo GetSecondaryAllocateInfo(
            std::uint32_t          count,
            const vk::CommandPool &pool) {
            return vk::CommandBufferAllocateInfo{
                pool,
                vk::CommandBufferLevel::eSecondary,
                count,
            };
        }

        vk::CommandBufferAllocateInfo GetPrimaryAllocateInfo(const vk::CommandPool &pool) {
            return vk::CommandBufferAllocateInfo{
                pool,
                vk::CommandBufferLevel::ePrimary,
                1,
            };
        }

        vk::CommandBufferAllocateInfo GetSecondaryAllocateInfo(const vk::CommandPool &pool) {
            return vk::CommandBufferAllocateInfo{
                pool,
                vk::CommandBufferLevel::eSecondary,
                1,
            };
        }

        std::vector<vk::CommandBuffer> AllocatePrimary(std::uint32_t count, const vk::CommandPool &pool, const vk::Device &device) {
            auto [result, buffers] = device.allocateCommandBuffers(vk::CommandBufferAllocateInfo{
                pool,
                vk::CommandBufferLevel::ePrimary,
                count,
            });
            VRE_VK_CHECK(result);
            DVRE_ASSERT(buffers.size() == count);
            return buffers;
        }

        std::vector<vk::CommandBuffer> AllocateSecondary(std::uint32_t count, const vk::CommandPool &pool, const vk::Device &device) {
            auto [result, buffers] = device.allocateCommandBuffers(vk::CommandBufferAllocateInfo{
                pool,
                vk::CommandBufferLevel::eSecondary,
                count,
            });
            VRE_VK_CHECK(result);
            DVRE_ASSERT(buffers.size() == count);
            return buffers;
        }

        vk::CommandBuffer AllocatePrimary(const vk::CommandPool &pool, const vk::Device &device) {
            auto [result, buffers] = device.allocateCommandBuffers(vk::CommandBufferAllocateInfo{
                pool,
                vk::CommandBufferLevel::ePrimary,
                1u,
            });
            VRE_VK_CHECK(result);
            DVRE_ASSERT(buffers.size() == 1);
            return buffers.front();
        }

        vk::CommandBuffer AllocateSecondary(const vk::CommandPool &pool, const vk::Device &device) {
            auto [result, buffers] = device.allocateCommandBuffers(vk::CommandBufferAllocateInfo{
                pool,
                vk::CommandBufferLevel::eSecondary,
                1,
            });
            VRE_VK_CHECK(result);
            DVRE_ASSERT(buffers.size() == 1);
            return buffers.front();
        }

        vk::CommandBufferBeginInfo GetBeginInfo(vk::CommandBufferUsageFlags flags) {
            return vk::CommandBufferBeginInfo{flags};
        }

        vk::CommandBufferBeginInfo GetBeginInfo() {
            return vk::CommandBufferBeginInfo{};
        }

        vk::CommandBufferBeginInfo GetOneTimeSubmitBeginInfo() {
            return vk::CommandBufferBeginInfo{vk::CommandBufferUsageFlagBits::eOneTimeSubmit};
        }

        vk::CommandBufferSubmitInfo GetSubmitInfo(const vk::CommandBuffer &buffer) {
            return vk::CommandBufferSubmitInfo{buffer};
        }

        void Begin(vk::CommandBufferUsageFlags flags, const vk::CommandBuffer &buffer) {
            VRE_VK_CHECK(buffer.begin(vk::CommandBufferBeginInfo{flags}));
        }

        void Begin(const vk::CommandBuffer &buffer) {
            VRE_VK_CHECK(buffer.begin(vk::CommandBufferBeginInfo{}));
        }

        void BeginOneTimeSubmit(const vk::CommandBuffer &buffer) {
            VRE_VK_CHECK(buffer.begin(vk::CommandBufferBeginInfo{vk::CommandBufferUsageFlagBits::eOneTimeSubmit}));
        }

        void End(const vk::CommandBuffer &buffer) {
            VRE_VK_CHECK(buffer.end());
        }

    }  // namespace CommandBuffer
}  // namespace vre::Vulkan