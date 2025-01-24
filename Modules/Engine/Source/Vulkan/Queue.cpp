#include <VREngine/Vulkan/Queue.hpp>

namespace vre::Vulkan::Queue {
    vk::SubmitInfo2 GetSubmitInfo(
        const vk::ArrayProxyNoTemporaries<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::ArrayProxyNoTemporaries<vk::SemaphoreSubmitInfo>     &waitSemaphores,
        const vk::ArrayProxyNoTemporaries<vk::SemaphoreSubmitInfo>     &signalSemaphores) {
        return vk::SubmitInfo2{
            {},
            waitSemaphores,
            bufferInfos,
            signalSemaphores,
        };
    }

    vk::SubmitInfo2 GetSubmitInfo(
        const vk::ArrayProxyNoTemporaries<vk::CommandBufferSubmitInfo> &bufferInfos) {
        return vk::SubmitInfo2{{}, {}, bufferInfos, {}};
    }

    vk::SubmitInfo2 GetWaitSubmitInfo(
        const vk::ArrayProxyNoTemporaries<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::ArrayProxyNoTemporaries<vk::SemaphoreSubmitInfo>     &waitSemaphores) {
        return vk::SubmitInfo2{
            {},
            waitSemaphores,
            bufferInfos,
            {},
        };
    }

    vk::SubmitInfo2 GetSignalSubmitInfo(
        const vk::ArrayProxyNoTemporaries<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::ArrayProxyNoTemporaries<vk::SemaphoreSubmitInfo>     &signalSemaphores) {
        return vk::SubmitInfo2{
            {},
            {},
            bufferInfos,
            signalSemaphores,
        };
    }

    vk::Result Present(
        const vk::ArrayProxy<vk::Semaphore> &waitSemaphores,
        const vk::SwapchainKHR              &swapchain,
        std::uint32_t                        imageIndex,
        const vk::Queue                     &queue) {
        return (vk::Result)vkQueuePresentKHR(
            queue,
            &(const VkPresentInfoKHR &)vk::PresentInfoKHR{
                waitSemaphores,
                {swapchain},
                {imageIndex},
            });
    }

    vk::Result Present(
        const vk::SwapchainKHR &swapchain,
        std::uint32_t           imageIndex,
        const vk::Queue        &queue) {
        return (vk::Result)vkQueuePresentKHR(
            queue,
            &(const VkPresentInfoKHR &)vk::PresentInfoKHR{
                {},
                {swapchain},
                {imageIndex},
            });
    }

    void Submit(
        const vk::ArrayProxy<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::ArrayProxy<vk::SemaphoreSubmitInfo>     &waitSemaphores,
        const vk::ArrayProxy<vk::SemaphoreSubmitInfo>     &signalSemaphores,
        const vk::Fence                                   &fence,
        const vk::Queue                                   &queue) {
        DVRE_VK_CHECK(queue.submit2(
            vk::SubmitInfo2{
                {},
                waitSemaphores,
                bufferInfos,
                signalSemaphores,
            },
            fence));
    }

    void Submit(
        const vk::ArrayProxy<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::Fence                                   &fence,
        const vk::Queue                                   &queue) {
        DVRE_VK_CHECK(queue.submit2(
            vk::SubmitInfo2{{}, {}, bufferInfos, {}},
            fence));
    }

    void SubmitWait(
        const vk::ArrayProxy<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::ArrayProxy<vk::SemaphoreSubmitInfo>     &waitSemaphores,
        const vk::Fence                                   &fence,
        const vk::Queue                                   &queue) {
        DVRE_VK_CHECK(queue.submit2(
            vk::SubmitInfo2{
                {},
                waitSemaphores,
                bufferInfos,
                {},
            },
            fence));
    }

    void SubmitSignal(
        const vk::ArrayProxy<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::ArrayProxy<vk::SemaphoreSubmitInfo>     &signalSemaphores,
        const vk::Fence                                   &fence,
        const vk::Queue                                   &queue) {
        DVRE_VK_CHECK(queue.submit2(
            vk::SubmitInfo2{
                {},
                {},
                bufferInfos,
                signalSemaphores,
            },
            fence));
    }
}  // namespace vre::Vulkan::Queue