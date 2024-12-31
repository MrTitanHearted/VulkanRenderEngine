#include <VREngine/Vulkan/Queue.hpp>

namespace vre::Vulkan::Queue {
    vk::SubmitInfo2 GetSubmitInfo(
        const std::span<vk::CommandBufferSubmitInfo> &bufferInfos,
        const std::span<vk::SemaphoreSubmitInfo>     &waitSemaphores,
        const std::span<vk::SemaphoreSubmitInfo>     &signalSemaphores) {
        return vk::SubmitInfo2{
            {},
            waitSemaphores,
            bufferInfos,
            signalSemaphores,
        };
    }

    vk::SubmitInfo2 GetSubmitInfo(const std::span<vk::CommandBufferSubmitInfo> &bufferInfos) {
        return vk::SubmitInfo2{{}, {}, bufferInfos, {}};
    }

    vk::SubmitInfo2 GetWaitSubmitInfo(
        const std::span<vk::CommandBufferSubmitInfo> &bufferInfos,
        const std::span<vk::SemaphoreSubmitInfo>     &waitSemaphores) {
        return vk::SubmitInfo2{
            {},
            waitSemaphores,
            bufferInfos,
            {},
        };
    }

    vk::SubmitInfo2 GetSignalSubmitInfo(
        const std::span<vk::CommandBufferSubmitInfo> &bufferInfos,
        const std::span<vk::SemaphoreSubmitInfo>     &signalSemaphores) {
        return vk::SubmitInfo2{
            {},
            {},
            bufferInfos,
            signalSemaphores,
        };
    }

    vk::Result Present(const std::span<vk::Semaphore> &waitSemaphores, const vk::SwapchainKHR &swapchain, std::uint32_t imageIndex, const vk::Queue &queue) {
        return queue.presentKHR(vk::PresentInfoKHR{
            waitSemaphores,
            {swapchain},
            {imageIndex},
        });
    }

    vk::Result Present(const vk::SwapchainKHR &swapchain, std::uint32_t imageIndex, const vk::Queue &queue) {
        return queue.presentKHR(vk::PresentInfoKHR{
            {},
            {swapchain},
            {imageIndex},
        });
    }

    void Submit(
        const std::vector<vk::CommandBufferSubmitInfo> &bufferInfos,
        const std::vector<vk::SemaphoreSubmitInfo>     &waitSemaphores,
        const std::vector<vk::SemaphoreSubmitInfo>     &signalSemaphores,
        const vk::Fence                                &fence,
        const vk::Queue                                &queue) {
        VRE_VK_CHECK(queue.submit2(
            vk::SubmitInfo2{
                {},
                waitSemaphores,
                bufferInfos,
                signalSemaphores,
            },
            fence));
    }

    void Submit(
        const std::vector<vk::CommandBufferSubmitInfo> &bufferInfos,
        const vk::Fence                                &fence,
        const vk::Queue                                &queue) {
        VRE_VK_CHECK(queue.submit2(
            vk::SubmitInfo2{{}, {}, bufferInfos, {}},
            fence));
    }

    void SubmitWait(
        const std::vector<vk::CommandBufferSubmitInfo> &bufferInfos,
        const std::vector<vk::SemaphoreSubmitInfo>     &waitSemaphores,
        const vk::Fence                                &fence,
        const vk::Queue                                &queue) {
        VRE_VK_CHECK(queue.submit2(
            vk::SubmitInfo2{
                {},
                waitSemaphores,
                bufferInfos,
                {},
            },
            fence));
    }

    void SubmitSignal(
        const std::vector<vk::CommandBufferSubmitInfo> &bufferInfos,
        const std::vector<vk::SemaphoreSubmitInfo>     &signalSemaphores,
        const vk::Fence                                &fence,
        const vk::Queue                                &queue) {
        VRE_VK_CHECK(queue.submit2(
            vk::SubmitInfo2{
                {},
                {},
                bufferInfos,
                signalSemaphores,
            },
            fence));
    }
}  // namespace vre::Vulkan::Queue