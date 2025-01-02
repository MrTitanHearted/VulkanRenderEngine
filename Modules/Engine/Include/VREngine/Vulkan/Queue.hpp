#pragma once

#include <VREngine/Core.hpp>

namespace vre::Vulkan::Queue {
    vk::SubmitInfo2 GetSubmitInfo(
        const vk::ArrayProxyNoTemporaries<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::ArrayProxyNoTemporaries<vk::SemaphoreSubmitInfo>     &waitSemaphores,
        const vk::ArrayProxyNoTemporaries<vk::SemaphoreSubmitInfo>     &signalSemaphores);
    vk::SubmitInfo2 GetSubmitInfo(
        const vk::ArrayProxyNoTemporaries<vk::CommandBufferSubmitInfo> &bufferInfos);
    vk::SubmitInfo2 GetWaitSubmitInfo(
        const vk::ArrayProxyNoTemporaries<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::ArrayProxyNoTemporaries<vk::SemaphoreSubmitInfo>     &waitSemaphores);
    vk::SubmitInfo2 GetSignalSubmitInfo(
        const vk::ArrayProxyNoTemporaries<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::ArrayProxyNoTemporaries<vk::SemaphoreSubmitInfo>     &signalSemaphores);

    vk::Result Present(
        const vk::ArrayProxy<vk::Semaphore> &waitSemaphores,
        const vk::SwapchainKHR              &swapchain,
        std::uint32_t                        imageIndex,
        const vk::Queue                     &queue);

    vk::Result Present(
        const vk::SwapchainKHR &swapchain,
        std::uint32_t           imageIndex,
        const vk::Queue        &queue);

    void Submit(
        const vk::ArrayProxy<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::ArrayProxy<vk::SemaphoreSubmitInfo>     &waitSemaphores,
        const vk::ArrayProxy<vk::SemaphoreSubmitInfo>     &signalSemaphores,
        const vk::Fence                                   &fence,
        const vk::Queue                                   &queue);

    void Submit(
        const vk::ArrayProxy<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::Fence                                   &fence,
        const vk::Queue                                   &queue);

    void SubmitWait(
        const vk::ArrayProxy<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::ArrayProxy<vk::SemaphoreSubmitInfo>     &waitSemaphores,
        const vk::Fence                                   &fence,
        const vk::Queue                                   &queue);

    void SubmitSignal(
        const vk::ArrayProxy<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::ArrayProxy<vk::SemaphoreSubmitInfo>     &signalSemaphores,
        const vk::Fence                                   &fence,
        const vk::Queue                                   &queue);
}  // namespace vre::Vulkan::Queue