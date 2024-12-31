#pragma once

#include <VREngine/Core.hpp>

namespace vre::Vulkan {
    namespace Fence {
        vk::FenceCreateInfo GetCreateInfo(vk::FenceCreateFlags flags);
        vk::FenceCreateInfo GetCreateInfo();
        vk::FenceCreateInfo GetSignaledCreateInfo();

        vk::Fence Create(vk::FenceCreateFlags flags, const vk::Device &device);
        vk::Fence Create(const vk::Device &device);
        vk::Fence CreateSignaled(const vk::Device &device);
    }  // namespace Fence

    namespace Semaphore {
        vk::SemaphoreCreateInfo GetCreateInfo(vk::SemaphoreCreateFlags flags);
        vk::SemaphoreCreateInfo GetCreateInfo();

        vk::Semaphore Create(vk::SemaphoreCreateFlags flags, const vk::Device &device);
        vk::Semaphore Create(const vk::Device &device);

        vk::SemaphoreSubmitInfo GetSubmitInfo(
            std::uint64_t           value,
            vk::PipelineStageFlags2 stageFlags,
            const vk::Semaphore    &semaphore);
        vk::SemaphoreSubmitInfo GetSubmitInfo(
            vk::PipelineStageFlags2 stageFlags,
            const vk::Semaphore    &semaphore);
    }  // namespace Semaphore
}  // namespace vre::Vulkan