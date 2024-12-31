#pragma once

#include <VREngine/Core.hpp>

namespace vre::Vulkan::Queue {
    vk::SubmitInfo2 GetSubmitInfo(
        const std::span<vk::CommandBufferSubmitInfo> &bufferInfos,
        const std::span<vk::SemaphoreSubmitInfo>     &waitSemaphores,
        const std::span<vk::SemaphoreSubmitInfo>     &signalSemaphores);
    vk::SubmitInfo2 GetSubmitInfo(
        const std::span<vk::CommandBufferSubmitInfo> &bufferInfos);
    vk::SubmitInfo2 GetWaitSubmitInfo(
        const std::span<vk::CommandBufferSubmitInfo> &bufferInfos,
        const std::span<vk::SemaphoreSubmitInfo>     &waitSemaphores);
    vk::SubmitInfo2 GetSignalSubmitInfo(
        const std::span<vk::CommandBufferSubmitInfo> &bufferInfos,
        const std::span<vk::SemaphoreSubmitInfo>     &signalSemaphores);

    vk::Result Present(
        const std::span<vk::Semaphore> &waitSemaphores,
        const vk::SwapchainKHR         &swapchain,
        std::uint32_t                   imageIndex,
        const vk::Queue                &queue);

    vk::Result Present(
        const vk::SwapchainKHR &swapchain,
        std::uint32_t           imageIndex,
        const vk::Queue        &queue);

    void Submit(
        const std::vector<vk::CommandBufferSubmitInfo> &bufferInfos,
        const std::vector<vk::SemaphoreSubmitInfo>     &waitSemaphores,
        const std::vector<vk::SemaphoreSubmitInfo>     &signalSemaphores,
        const vk::Fence                                &fence,
        const vk::Queue                                &queue);

    void Submit(
        const std::vector<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::Fence                                &fence,
        const vk::Queue                                &queue);

    void SubmitWait(
        const std::vector<vk::CommandBufferSubmitInfo> &bufferInfos,
        const std::vector<vk::SemaphoreSubmitInfo>     &waitSemaphores,
        const vk::Fence                                &fence,
        const vk::Queue                                &queue);

    void SubmitSignal(
        const std::vector<vk::CommandBufferSubmitInfo> &bufferInfos,
        const std::vector<vk::SemaphoreSubmitInfo>     &signalSemaphores,
        const vk::Fence                                &fence,
        const vk::Queue                                &queue);
}  // namespace vre::Vulkan::Queue