#include <VREngine/Vulkan/RenderPass.hpp>

namespace vre::Vulkan::RenderPass {
    vk::RenderingAttachmentInfo GetAttachmentInfo(
        const vk::ImageView  &view,
        vk::ImageLayout       layout,
        const vk::ClearValue &clearValue) {
        return vk::RenderingAttachmentInfo{
            view,
            layout,
            vk::ResolveModeFlagBits::eNone,
            {},
            vk::ImageLayout::eUndefined,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            clearValue,
        };
    }

    vk::RenderingAttachmentInfo GetAttachmentInfo(
        const vk::ImageView &view,
        vk::ImageLayout      layout) {
        return vk::RenderingAttachmentInfo{
            view,
            layout,
            vk::ResolveModeFlagBits::eNone,
            {},
            vk::ImageLayout::eUndefined,
            vk::AttachmentLoadOp::eLoad,
            vk::AttachmentStoreOp::eStore,
        };
    }

    vk::RenderingInfo GetRenderingInfo(
        const vk::Extent2D                                             &extent,
        const vk::ArrayProxyNoTemporaries<vk::RenderingAttachmentInfo> &colorAttachments,
        const vk::RenderingAttachmentInfo                               depthAttachment) {
        return vk::RenderingInfo{
            {},
            vk::Rect2D{vk::Offset2D{0, 0}, extent},
            1u,
            0u,
            colorAttachments,
            &depthAttachment,
        };
    }
    vk::RenderingInfo GetRenderingInfo(
        const vk::Extent2D                                             &extent,
        const vk::ArrayProxyNoTemporaries<vk::RenderingAttachmentInfo> &colorAttachments) {
        return vk::RenderingInfo{
            {},
            vk::Rect2D{vk::Offset2D{0, 0}, extent},
            1u,
            0u,
            colorAttachments,
        };
    }

    void BeginRendering(
        const vk::CommandBuffer                           &commandBuffer,
        const vk::Extent2D                                &extent,
        const vk::ArrayProxy<vk::RenderingAttachmentInfo> &colorAttachments,
        const vk::RenderingAttachmentInfo                  depthAttachment) {
        commandBuffer.beginRendering(vk::RenderingInfo{
            {},
            vk::Rect2D{vk::Offset2D{0, 0}, extent},
            1u,
            0u,
            colorAttachments,
            &depthAttachment,
        });
    }

    void BeginRendering(
        const vk::CommandBuffer                           &commandBuffer,
        const vk::Extent2D                                &extent,
        const vk::ArrayProxy<vk::RenderingAttachmentInfo> &colorAttachments) {
        commandBuffer.beginRendering(vk::RenderingInfo{
            {},
            vk::Rect2D{vk::Offset2D{0, 0}, extent},
            1u,
            0u,
            colorAttachments,
        });
    }

    void EndRendering(const vk::CommandBuffer &commandBuffer) {
        commandBuffer.endRendering();
    }
}  // namespace vre::Vulkan::RenderPass