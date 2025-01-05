#pragma once

#include <VREngine/Core.hpp>

namespace vre::Vulkan::RenderPass {
    vk::RenderingAttachmentInfo GetAttachmentInfo(
        const vk::ImageView  &view,
        vk::ImageLayout       layout,
        const vk::ClearValue &clearValue);
    vk::RenderingAttachmentInfo GetAttachmentInfo(
        const vk::ImageView &view,
        vk::ImageLayout      layout);

    vk::RenderingInfo GetRenderingInfo(
        const vk::Extent2D                                             &extent,
        const vk::ArrayProxyNoTemporaries<vk::RenderingAttachmentInfo> &colorAttachments,
        const vk::RenderingAttachmentInfo                               depthAttachment);
    vk::RenderingInfo GetRenderingInfo(
        const vk::Extent2D                                             &extent,
        const vk::ArrayProxyNoTemporaries<vk::RenderingAttachmentInfo> &colorAttachments);

    void BeginRendering(
        const vk::CommandBuffer                           &commandBuffer,
        const vk::Extent2D                                &extent,
        const vk::ArrayProxy<vk::RenderingAttachmentInfo> &colorAttachments,
        const vk::RenderingAttachmentInfo                  depthAttachment);
    void BeginRendering(
        const vk::CommandBuffer                           &commandBuffer,
        const vk::Extent2D                                &extent,
        const vk::ArrayProxy<vk::RenderingAttachmentInfo> &colorAttachments);

    void EndRendering(const vk::CommandBuffer &commandBuffer);
}  // namespace vre::Vulkan::RenderPass