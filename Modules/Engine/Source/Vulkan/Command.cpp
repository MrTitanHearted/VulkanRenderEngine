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
            DVRE_VK_CHECK(result);
            return pool;
        }

        vk::CommandPool Create(std::uint32_t queueFamilyIndex, const vk::Device &device) {
            auto [result, pool] = device.createCommandPool(vk::CommandPoolCreateInfo{
                {},
                queueFamilyIndex,
            });
            DVRE_VK_CHECK(result);
            return pool;
        }

        vk::CommandPool CreateTransient(std::uint32_t queueFamilyIndex, const vk::Device &device) {
            auto [result, pool] = device.createCommandPool(vk::CommandPoolCreateInfo{
                vk::CommandPoolCreateFlagBits::eTransient,
                queueFamilyIndex,
            });
            DVRE_VK_CHECK(result);
            return pool;
        }

        vk::CommandPool CreateResetCommandBuffer(
            std::uint32_t     queueFamilyIndex,
            const vk::Device &device) {
            auto [result, pool] = device.createCommandPool(vk::CommandPoolCreateInfo{
                vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                queueFamilyIndex,
            });
            DVRE_VK_CHECK(result);
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
            DVRE_VK_CHECK(result);
            DVRE_ASSERT(buffers.size() == count);
            return buffers;
        }

        std::vector<vk::CommandBuffer> AllocateSecondary(std::uint32_t count, const vk::CommandPool &pool, const vk::Device &device) {
            auto [result, buffers] = device.allocateCommandBuffers(vk::CommandBufferAllocateInfo{
                pool,
                vk::CommandBufferLevel::eSecondary,
                count,
            });
            DVRE_VK_CHECK(result);
            DVRE_ASSERT(buffers.size() == count);
            return buffers;
        }

        vk::CommandBuffer AllocatePrimary(const vk::CommandPool &pool, const vk::Device &device) {
            auto [result, buffers] = device.allocateCommandBuffers(vk::CommandBufferAllocateInfo{
                pool,
                vk::CommandBufferLevel::ePrimary,
                1u,
            });
            DVRE_VK_CHECK(result);
            DVRE_ASSERT(buffers.size() == 1);
            return buffers.front();
        }

        vk::CommandBuffer AllocateSecondary(const vk::CommandPool &pool, const vk::Device &device) {
            auto [result, buffers] = device.allocateCommandBuffers(vk::CommandBufferAllocateInfo{
                pool,
                vk::CommandBufferLevel::eSecondary,
                1,
            });
            DVRE_VK_CHECK(result);
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
            DVRE_VK_CHECK(buffer.begin(vk::CommandBufferBeginInfo{flags}));
        }

        void Begin(const vk::CommandBuffer &buffer) {
            DVRE_VK_CHECK(buffer.begin(vk::CommandBufferBeginInfo{}));
        }

        void BeginOneTimeSubmit(const vk::CommandBuffer &buffer) {
            DVRE_VK_CHECK(buffer.begin(vk::CommandBufferBeginInfo{vk::CommandBufferUsageFlagBits::eOneTimeSubmit}));
        }

        void End(const vk::CommandBuffer &buffer) {
            DVRE_VK_CHECK(buffer.end());
        }

        void CopyImageToImage(
            const vk::CommandBuffer &buffer,
            const Image::Allocation &source,
            const Image::Allocation &destination) {
            vk::ImageBlit2 imageBlit{
                vk::ImageSubresourceLayers{
                    vk::ImageAspectFlagBits::eColor,
                    0u,
                    0u,
                    1u,
                },
                {
                    vk::Offset3D{0, 0, 0},
                    vk::Offset3D{
                        std::int32_t(source.Extent.width),
                        std::int32_t(source.Extent.height),
                        std::int32_t(source.Extent.depth),
                    },
                },
                vk::ImageSubresourceLayers{
                    vk::ImageAspectFlagBits::eColor,
                    0u,
                    0u,
                    1u,
                },
                {
                    vk::Offset3D{0, 0, 0},
                    vk::Offset3D{
                        std::int32_t(destination.Extent.width),
                        std::int32_t(destination.Extent.height),
                        std::int32_t(destination.Extent.depth),
                    },
                },
            };
            buffer.blitImage2(vk::BlitImageInfo2{
                source.Image,
                vk::ImageLayout::eTransferSrcOptimal,
                destination.Image,
                vk::ImageLayout::eTransferDstOptimal,
                {imageBlit},
                vk::Filter::eLinear,
            });
        }

        void CopyImageToImage(
            const vk::CommandBuffer &buffer,
            const vk::Image         &source,
            const vk::Image         &destination,
            const vk::Extent3D      &size) {
            vk::ImageBlit2 imageBlit{
                vk::ImageSubresourceLayers{
                    vk::ImageAspectFlagBits::eColor,
                    0u,
                    0u,
                    1u,
                },
                {
                    vk::Offset3D{0, 0, 0},
                    vk::Offset3D{
                        std::int32_t(size.width),
                        std::int32_t(size.height),
                        std::int32_t(size.depth),
                    },
                },
                vk::ImageSubresourceLayers{
                    vk::ImageAspectFlagBits::eColor,
                    0u,
                    0u,
                    1u,
                },
                {
                    vk::Offset3D{0, 0, 0},
                    vk::Offset3D{
                        std::int32_t(size.width),
                        std::int32_t(size.height),
                        std::int32_t(size.depth),
                    },
                },
            };
            buffer.blitImage2(vk::BlitImageInfo2{
                source,
                vk::ImageLayout::eTransferSrcOptimal,
                destination,
                vk::ImageLayout::eTransferDstOptimal,
                {imageBlit},
                vk::Filter::eLinear,
            });
        }

        void CopyImageToImage(
            const vk::CommandBuffer &buffer,
            const vk::Image         &source,
            const vk::Image         &destination,
            const vk::Extent2D      &size) {
            vk::ImageBlit2 imageBlit{
                vk::ImageSubresourceLayers{
                    vk::ImageAspectFlagBits::eColor,
                    0u,
                    0u,
                    1u,
                },
                {
                    vk::Offset3D{0, 0, 0},
                    vk::Offset3D{
                        std::int32_t(size.width),
                        std::int32_t(size.height),
                        1,
                    },
                },
                vk::ImageSubresourceLayers{
                    vk::ImageAspectFlagBits::eColor,
                    0u,
                    0u,
                    1u,
                },
                {
                    vk::Offset3D{0, 0, 0},
                    vk::Offset3D{
                        std::int32_t(size.width),
                        std::int32_t(size.height),
                        1,
                    },
                },
            };
            buffer.blitImage2(vk::BlitImageInfo2{
                source,
                vk::ImageLayout::eTransferSrcOptimal,
                destination,
                vk::ImageLayout::eTransferDstOptimal,
                {imageBlit},
                vk::Filter::eLinear,
            });
        }

        void CopyImageToImage(
            const vk::CommandBuffer &buffer,
            const vk::Image         &source,
            const vk::Image         &destination,
            const vk::Extent3D      &sourceExtent,
            const vk::Extent3D      &destinationExtent) {
            vk::ImageBlit2 imageBlit{
                vk::ImageSubresourceLayers{
                    vk::ImageAspectFlagBits::eColor,
                    0u,
                    0u,
                    1u,
                },
                {
                    vk::Offset3D{0, 0, 0},
                    vk::Offset3D{
                        std::int32_t(sourceExtent.width),
                        std::int32_t(sourceExtent.height),
                        std::int32_t(sourceExtent.depth),
                    },
                },
                vk::ImageSubresourceLayers{
                    vk::ImageAspectFlagBits::eColor,
                    0u,
                    0u,
                    1u,
                },
                {
                    vk::Offset3D{0, 0, 0},
                    vk::Offset3D{
                        std::int32_t(destinationExtent.width),
                        std::int32_t(destinationExtent.height),
                        std::int32_t(destinationExtent.depth),
                    },
                },
            };
            buffer.blitImage2(vk::BlitImageInfo2{
                source,
                vk::ImageLayout::eTransferSrcOptimal,
                destination,
                vk::ImageLayout::eTransferDstOptimal,
                {imageBlit},
                vk::Filter::eLinear,
            });
        }

        void CopyImageToImage(
            const vk::CommandBuffer &buffer,
            const vk::Image         &source,
            const vk::Image         &destination,
            const vk::Extent2D      &sourceExtent,
            const vk::Extent2D      &destinationExtent) {
            vk::ImageBlit2 imageBlit{
                vk::ImageSubresourceLayers{
                    vk::ImageAspectFlagBits::eColor,
                    0u,
                    0u,
                    1u,
                },
                {
                    vk::Offset3D{0, 0, 0},
                    vk::Offset3D{
                        std::int32_t(sourceExtent.width),
                        std::int32_t(sourceExtent.height),
                        1,
                    },
                },
                vk::ImageSubresourceLayers{
                    vk::ImageAspectFlagBits::eColor,
                    0u,
                    0u,
                    1u,
                },
                {
                    vk::Offset3D{0, 0, 0},
                    vk::Offset3D{
                        std::int32_t(destinationExtent.width),
                        std::int32_t(destinationExtent.height),
                        1,
                    },
                },
            };
            buffer.blitImage2(vk::BlitImageInfo2{
                source,
                vk::ImageLayout::eTransferSrcOptimal,
                destination,
                vk::ImageLayout::eTransferDstOptimal,
                {imageBlit},
                vk::Filter::eLinear,
            });
        }

        void CopyBufferToBuffer(
            const vk::CommandBuffer  &buffer,
            const Buffer::Allocation &source,
            const Buffer::Allocation &destination,
            std::uint64_t             sourceOffset,
            std::uint64_t             destinationOffset,
            std::uint64_t             size) {
            buffer.copyBuffer(
                source.Buffer,
                destination.Buffer,
                {vk::BufferCopy{
                    sourceOffset,
                    destinationOffset,
                    size,
                }});
        }

        void CopyBufferToBuffer(
            const vk::CommandBuffer  &buffer,
            const Buffer::Allocation &source,
            const Buffer::Allocation &destination,
            std::uint64_t             size) {
            buffer.copyBuffer(
                source.Buffer,
                destination.Buffer,
                {vk::BufferCopy{
                    0U,
                    0U,
                    size,
                }});
        }

        void CopyBufferToBuffer(
            const vk::CommandBuffer &buffer,
            const vk::Buffer        &source,
            const vk::Buffer        &destination,
            std::uint64_t            sourceOffset,
            std::uint64_t            destinationOffset,
            std::uint64_t            size) {
            buffer.copyBuffer(
                source,
                destination,
                {vk::BufferCopy{
                    sourceOffset,
                    destinationOffset,
                    size,
                }});
        }

        void CopyBufferToBuffer(
            const vk::CommandBuffer &buffer,
            const vk::Buffer        &source,
            const vk::Buffer        &destination,
            std::uint64_t            size) {
            buffer.copyBuffer(
                source,
                destination,
                {vk::BufferCopy{
                    0U,
                    0U,
                    size,
                }});
        }

        void CopyBufferToImage(
            const vk::CommandBuffer  &buffer,
            const Buffer::Allocation &source,
            const Image::Allocation  &destination) {
            buffer.copyBufferToImage(
                source.Buffer,
                destination.Image,
                vk::ImageLayout::eTransferDstOptimal,
                {vk::BufferImageCopy{
                    0u,
                    0u,
                    0u,
                    vk::ImageSubresourceLayers{
                        vk::ImageAspectFlagBits::eColor,
                        0u,
                        0u,
                        1u,
                    },
                    vk::Offset3D{0, 0, 0},
                    destination.Extent,
                }});
        }

        void CopyBufferToImage(
            const vk::CommandBuffer  &buffer,
            const Buffer::Allocation &source,
            const vk::Image          &destination,
            const vk::Extent3D       &size) {
            buffer.copyBufferToImage(
                source.Buffer,
                destination,
                vk::ImageLayout::eTransferDstOptimal,
                {vk::BufferImageCopy{
                    0u,
                    0u,
                    0u,
                    vk::ImageSubresourceLayers{
                        vk::ImageAspectFlagBits::eColor,
                        0u,
                        0u,
                        1u,
                    },
                    vk::Offset3D{0, 0, 0},
                    size,
                }});
        }

        void CopyBufferToImage(
            const vk::CommandBuffer &buffer,
            const vk::Buffer        &source,
            const Image::Allocation &destination) {
            buffer.copyBufferToImage(
                source,
                destination.Image,
                vk::ImageLayout::eTransferDstOptimal,
                {vk::BufferImageCopy{
                    0u,
                    0u,
                    0u,
                    vk::ImageSubresourceLayers{
                        vk::ImageAspectFlagBits::eColor,
                        0u,
                        0u,
                        1u,
                    },
                    vk::Offset3D{0, 0, 0},
                    destination.Extent,
                }});
        }

        void CopyBufferToImage(
            const vk::CommandBuffer &buffer,
            const vk::Buffer        &source,
            const vk::Image         &destination,
            const vk::Extent3D      &size) {
            buffer.copyBufferToImage(
                source,
                destination,
                vk::ImageLayout::eTransferDstOptimal,
                {vk::BufferImageCopy{
                    0u,
                    0u,
                    0u,
                    vk::ImageSubresourceLayers{
                        vk::ImageAspectFlagBits::eColor,
                        0u,
                        0u,
                        1u,
                    },
                    vk::Offset3D{0, 0, 0},
                    size,
                }});
        }

        void CopyImageToBuffer(
            const vk::CommandBuffer  &buffer,
            const Image::Allocation  &source,
            const Buffer::Allocation &destination) {
            buffer.copyImageToBuffer(
                source.Image,
                vk::ImageLayout::eTransferSrcOptimal,
                destination.Buffer,
                {vk::BufferImageCopy{
                    0u,
                    0u,
                    0u,
                    vk::ImageSubresourceLayers{
                        vk::ImageAspectFlagBits::eColor,
                        0u,
                        0u,
                        1u,
                    },
                    vk::Offset3D{0, 0, 0},
                    source.Extent,
                }});
        }

        void CopyImageToBuffer(
            const vk::CommandBuffer &buffer,
            const Image::Allocation &source,
            const vk::Buffer        &destination) {
            buffer.copyImageToBuffer(
                source.Image,
                vk::ImageLayout::eTransferSrcOptimal,
                destination,
                {vk::BufferImageCopy{
                    0u,
                    0u,
                    0u,
                    vk::ImageSubresourceLayers{
                        vk::ImageAspectFlagBits::eColor,
                        0u,
                        0u,
                        1u,
                    },
                    vk::Offset3D{0, 0, 0},
                    source.Extent,
                }});
        }

        void CopyImageToBuffer(
            const vk::CommandBuffer  &buffer,
            const vk::Image          &source,
            const Buffer::Allocation &destination,
            const vk::Extent3D       &size) {
            buffer.copyImageToBuffer(
                source,
                vk::ImageLayout::eTransferSrcOptimal,
                destination.Buffer,
                {vk::BufferImageCopy{
                    0u,
                    0u,
                    0u,
                    vk::ImageSubresourceLayers{
                        vk::ImageAspectFlagBits::eColor,
                        0u,
                        0u,
                        1u,
                    },
                    vk::Offset3D{0, 0, 0},
                    size,
                }});
        }

        void CopyImageToBuffer(
            const vk::CommandBuffer &buffer,
            const vk::Image         &source,
            const vk::Buffer        &destination,
            const vk::Extent3D      &size) {
            buffer.copyImageToBuffer(
                source,
                vk::ImageLayout::eTransferSrcOptimal,
                destination,
                {vk::BufferImageCopy{
                    0u,
                    0u,
                    0u,
                    vk::ImageSubresourceLayers{
                        vk::ImageAspectFlagBits::eColor,
                        0u,
                        0u,
                        1u,
                    },
                    vk::Offset3D{0, 0, 0},
                    size,
                }});
        }
    }  // namespace CommandBuffer
}  // namespace vre::Vulkan