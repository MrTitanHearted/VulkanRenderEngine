#include <VREngine/Vulkan/Synchronization.hpp>

namespace vre::Vulkan {
    namespace Fence {
        vk::FenceCreateInfo GetCreateInfo(vk::FenceCreateFlags flags) {
            return vk::FenceCreateInfo{flags};
        }

        vk::FenceCreateInfo GetCreateInfo() {
            return vk::FenceCreateInfo{};
        }

        vk::FenceCreateInfo GetSignaledCreateInfo() {
            return vk::FenceCreateInfo{vk::FenceCreateFlagBits::eSignaled};
        }

        vk::Fence Create(vk::FenceCreateFlags flags, const vk::Device &device) {
            auto [result, fence] = device.createFence(vk::FenceCreateInfo{flags});
            DVRE_VK_CHECK(result);
            return fence;
        }

        vk::Fence Create(const vk::Device &device) {
            auto [result, fence] = device.createFence(vk::FenceCreateInfo{});
            DVRE_VK_CHECK(result);
            return fence;
        }

        vk::Fence CreateSignaled(const vk::Device &device) {
            auto [result, fence] = device.createFence(vk::FenceCreateInfo{vk::FenceCreateFlagBits::eSignaled});
            DVRE_VK_CHECK(result);
            return fence;
        }
    }  // namespace Fence

    namespace Semaphore {
        vk::SemaphoreCreateInfo GetCreateInfo(vk::SemaphoreCreateFlags flags) {
            return vk::SemaphoreCreateInfo{flags};
        }

        vk::SemaphoreCreateInfo GetCreateInfo() {
            return vk::SemaphoreCreateInfo{};
        }

        vk::Semaphore Create(vk::SemaphoreCreateFlags flags, const vk::Device &device) {
            auto [result, semaphore] = device.createSemaphore(vk::SemaphoreCreateInfo{flags});
            DVRE_VK_CHECK(result);
            return semaphore;
        }

        vk::Semaphore Create(const vk::Device &device) {
            auto [result, semaphore] = device.createSemaphore(vk::SemaphoreCreateInfo{});
            DVRE_VK_CHECK(result);
            return semaphore;
        }

        vk::SemaphoreSubmitInfo GetSubmitInfo(
            std::uint64_t           value,
            vk::PipelineStageFlags2 stageFlags,
            const vk::Semaphore    &semaphore) {
            return vk::SemaphoreSubmitInfo{
                semaphore,
                value,
                stageFlags,
                0u,
            };
        }

        vk::SemaphoreSubmitInfo GetSubmitInfo(
            vk::PipelineStageFlags2 stageFlags,
            const vk::Semaphore    &semaphore) {
            return vk::SemaphoreSubmitInfo{
                semaphore,
                1U,
                stageFlags,
                0u,
            };
        }
    }  // namespace Semaphore
}  // namespace vre::Vulkan