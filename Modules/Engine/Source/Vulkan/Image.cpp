#include <VREngine/Vulkan/Image.hpp>

namespace vre::Vulkan::Image {
    void TransitionLayout(
        const vk::CommandBuffer &commandBuffer,
        const Allocation        &image,
        vk::ImageLayout          oldLayout,
        vk::ImageLayout          newLayout) {
        vk::ImageMemoryBarrier2 imageBarrier{
            vk::PipelineStageFlagBits2::eAllCommands,
            vk::AccessFlagBits2::eMemoryWrite,
            vk::PipelineStageFlagBits2::eAllCommands,
            vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead,
            oldLayout,
            newLayout,
        };
        imageBarrier.setImage(image.Image);
        imageBarrier.setSubresourceRange(vk::ImageSubresourceRange{
            newLayout == vk::ImageLayout::eDepthAttachmentOptimal
                ? vk::ImageAspectFlagBits::eDepth
                : vk::ImageAspectFlagBits::eColor,
            0u,
            vk::RemainingMipLevels,
            0u,
            vk::RemainingArrayLayers,
        });
        commandBuffer.pipelineBarrier2({vk::DependencyInfo{{}, {}, {}, {imageBarrier}}});
    }

    void TransitionLayout(const vk::CommandBuffer &commandBuffer, const Allocation &image, vk::ImageLayout newLayout) {
        vk::ImageMemoryBarrier2 imageBarrier{
            vk::PipelineStageFlagBits2::eAllCommands,
            vk::AccessFlagBits2::eMemoryWrite,
            vk::PipelineStageFlagBits2::eAllCommands,
            vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead,
            vk::ImageLayout::eUndefined,
            newLayout,
        };
        imageBarrier.setImage(image.Image);
        imageBarrier.setSubresourceRange(vk::ImageSubresourceRange{
            newLayout == vk::ImageLayout::eDepthAttachmentOptimal
                ? vk::ImageAspectFlagBits::eDepth
                : vk::ImageAspectFlagBits::eColor,
            0u,
            vk::RemainingMipLevels,
            0u,
            vk::RemainingArrayLayers,
        });
        commandBuffer.pipelineBarrier2({vk::DependencyInfo{{}, {}, {}, {imageBarrier}}});
    }

    void TransitionLayout(const vk::CommandBuffer &commandBuffer, const vk::Image &image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
        vk::ImageMemoryBarrier2 imageBarrier{
            vk::PipelineStageFlagBits2::eAllCommands,
            vk::AccessFlagBits2::eMemoryWrite,
            vk::PipelineStageFlagBits2::eAllCommands,
            vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead,
            oldLayout,
            newLayout,
        };
        imageBarrier.setImage(image);
        imageBarrier.setSubresourceRange(vk::ImageSubresourceRange{
            newLayout == vk::ImageLayout::eDepthAttachmentOptimal
                ? vk::ImageAspectFlagBits::eDepth
                : vk::ImageAspectFlagBits::eColor,
            0u,
            vk::RemainingMipLevels,
            0u,
            vk::RemainingArrayLayers,
        });
        commandBuffer.pipelineBarrier2({vk::DependencyInfo{{}, {}, {}, {imageBarrier}}});
    }

    void TransitionLayout(const vk::CommandBuffer &commandBuffer, const vk::Image &image, vk::ImageLayout newLayout) {
        vk::ImageMemoryBarrier2 imageBarrier{
            vk::PipelineStageFlagBits2::eAllCommands,
            vk::AccessFlagBits2::eMemoryWrite,
            vk::PipelineStageFlagBits2::eAllCommands,
            vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead,
            vk::ImageLayout::eUndefined,
            newLayout,
        };
        imageBarrier.setImage(image);
        imageBarrier.setSubresourceRange(vk::ImageSubresourceRange{
            newLayout == vk::ImageLayout::eDepthAttachmentOptimal
                ? vk::ImageAspectFlagBits::eDepth
                : vk::ImageAspectFlagBits::eColor,
            0u,
            vk::RemainingMipLevels,
            0u,
            vk::RemainingArrayLayers,
        });
        commandBuffer.pipelineBarrier2({vk::DependencyInfo{{}, {}, {}, {imageBarrier}}});
    }

    void Release(const Allocation &image) {
        vmaDestroyImage(image.Allocator, image.Image, image.Allocation);
    }

    vk::ImageSubresourceRange GetSubresourceRange(vk::ImageAspectFlags aspectFlags) {
        return vk::ImageSubresourceRange{
            aspectFlags,
            0u,
            vk::RemainingMipLevels,
            0u,
            vk::RemainingArrayLayers,
        };
    }

    vk::ImageSubresourceRange GetColorRange() {
        return vk::ImageSubresourceRange{
            vk::ImageAspectFlagBits::eColor,
            0u,
            vk::RemainingMipLevels,
            0u,
            vk::RemainingArrayLayers,
        };
    }

    vk::ImageSubresourceRange GetDepthRange() {
        return vk::ImageSubresourceRange{
            vk::ImageAspectFlagBits::eDepth,
            0u,
            vk::RemainingMipLevels,
            0u,
            vk::RemainingArrayLayers,
        };
    }

    vk::ImageCreateInfo GetCreateInfo(
        vk::Format          format,
        vk::ImageUsageFlags usageFlags,
        const vk::Extent3D &extent) {
        return vk::ImageCreateInfo{
            {},
            vk::ImageType::e3D,
            format,
            extent,
            1u,
            1u,
            vk::SampleCountFlagBits::e1,
            vk::ImageTiling::eOptimal,
            usageFlags,
        };
    }

    vk::ImageCreateInfo GetCreateInfo(
        vk::Format          format,
        vk::ImageUsageFlags usageFlags,
        const vk::Extent2D &extent) {
        return vk::ImageCreateInfo{
            {},
            vk::ImageType::e2D,
            format,
            vk::Extent3D{extent, 1u},
            1u,
            1u,
            vk::SampleCountFlagBits::e1,
            vk::ImageTiling::eOptimal,
            usageFlags,
        };
    }

    vk::ImageViewCreateInfo GetViewCreateInfo(
        const Allocation    &image,
        vk::ImageAspectFlags aspectFlags) {
        return vk::ImageViewCreateInfo{
            {},
            image.Image,
            image.Extent.depth == 1
                ? vk::ImageViewType::e2D
                : vk::ImageViewType::e3D,
            image.Format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                aspectFlags,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            }};
    }

    vk::ImageViewCreateInfo GetColorViewCreateInfo(const Allocation &image) {
        return vk::ImageViewCreateInfo{
            {},
            image.Image,
            image.Extent.depth == 1
                ? vk::ImageViewType::e2D
                : vk::ImageViewType::e3D,
            image.Format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eColor,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            }};
    }

    vk::ImageViewCreateInfo GetDepthViewCreateInfo(const Allocation &image) {
        return vk::ImageViewCreateInfo{
            {},
            image.Image,
            image.Extent.depth == 1
                ? vk::ImageViewType::e2D
                : vk::ImageViewType::e3D,
            image.Format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eDepth,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            }};
    }

    vk::ImageViewCreateInfo GetViewCreateInfo3D(
        const vk::Image     &image,
        vk::Format           format,
        vk::ImageAspectFlags aspectFlags) {
        return vk::ImageViewCreateInfo{
            {},
            image,
            vk::ImageViewType::e3D,
            format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                aspectFlags,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            }};
    }

    vk::ImageViewCreateInfo GetColorViewCreateInfo3D(const vk::Image &image, vk::Format format) {
        return vk::ImageViewCreateInfo{
            {},
            image,
            vk::ImageViewType::e3D,
            format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eColor,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            }};
    }

    vk::ImageViewCreateInfo GetDepthViewCreateInfo3D(const vk::Image &image, vk::Format format) {
        return vk::ImageViewCreateInfo{
            {},
            image,
            vk::ImageViewType::e3D,
            format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eDepth,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            }};
    }

    vk::ImageViewCreateInfo GetViewCreateInfo2D(
        const vk::Image     &image,
        vk::Format           format,
        vk::ImageAspectFlags aspectFlags) {
        return vk::ImageViewCreateInfo{
            {},
            image,
            vk::ImageViewType::e2D,
            format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                aspectFlags,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            }};
    }

    vk::ImageViewCreateInfo GetColorViewCreateInfo2D(const vk::Image &image, vk::Format format) {
        return vk::ImageViewCreateInfo{
            {},
            image,
            vk::ImageViewType::e2D,
            format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eColor,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            }};
    }

    vk::ImageViewCreateInfo GetDepthViewCreateInfo2D(const vk::Image &image, vk::Format format) {
        return vk::ImageViewCreateInfo{
            {},
            image,
            vk::ImageViewType::e2D,
            format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eDepth,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            }};
    }

    Allocation Allocate(
        VmaAllocationCreateFlags vmaFlags,
        vk::MemoryPropertyFlags  memoryFlags,
        vk::Format               format,
        vk::ImageUsageFlags      usageFlags,
        const vk::Extent3D      &extent,
        const VmaAllocator      &allocator) {
        vk::ImageCreateInfo imageInfo{
            {},
            vk::ImageType::e3D,
            format,
            extent,
            1u,
            1u,
            vk::SampleCountFlagBits::e1,
            vk::ImageTiling::eOptimal,
            usageFlags,
        };
        VmaAllocationCreateInfo allocationInfo{
            .flags         = vmaFlags,
            .usage         = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
            .requiredFlags = VkMemoryPropertyFlags(memoryFlags),
        };

        VkImage       cImage{VK_NULL_HANDLE};
        VmaAllocation allocation{VK_NULL_HANDLE};

        DVRE_VK_CHECK(vmaCreateImage(
            allocator,
            &(const VkImageCreateInfo &)imageInfo,
            &allocationInfo,
            &cImage,
            &allocation,
            nullptr));

        return Allocation{
            .Image      = cImage,
            .Extent     = extent,
            .Format     = format,
            .Allocation = allocation,
            .Allocator  = allocator,
        };
    }

    Allocation Allocate(
        VmaAllocationCreateFlags vmaFlags,
        vk::MemoryPropertyFlags  memoryFlags,
        vk::Format               format,
        vk::ImageUsageFlags      usageFlags,
        const vk::Extent2D      &extent,
        const VmaAllocator      &allocator) {
        vk::ImageCreateInfo imageInfo{
            {},
            vk::ImageType::e2D,
            format,
            vk::Extent3D{extent, 1u},
            1u,
            1u,
            vk::SampleCountFlagBits::e1,
            vk::ImageTiling::eOptimal,
            usageFlags,
        };
        VmaAllocationCreateInfo allocationInfo{
            .flags         = vmaFlags,
            .usage         = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
            .requiredFlags = VkMemoryPropertyFlags(memoryFlags),
        };

        VkImage       cImage{VK_NULL_HANDLE};
        VmaAllocation allocation{VK_NULL_HANDLE};

        DVRE_VK_CHECK(vmaCreateImage(
            allocator,
            &(const VkImageCreateInfo &)imageInfo,
            &allocationInfo,
            &cImage,
            &allocation,
            nullptr));

        return Allocation{
            .Image      = cImage,
            .Extent     = vk::Extent3D{extent, 1u},
            .Format     = format,
            .Allocation = allocation,
            .Allocator  = allocator,
        };
    }

    Allocation Allocate(
        vk::MemoryPropertyFlags memoryFlags,
        vk::Format              format,
        vk::ImageUsageFlags     usageFlags,
        const vk::Extent3D     &extent,
        const VmaAllocator     &allocator) {
        vk::ImageCreateInfo imageInfo{
            {},
            vk::ImageType::e3D,
            format,
            extent,
            1u,
            1u,
            vk::SampleCountFlagBits::e1,
            vk::ImageTiling::eOptimal,
            usageFlags,
        };
        VmaAllocationCreateInfo allocationInfo{
            .usage         = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
            .requiredFlags = VkMemoryPropertyFlags(memoryFlags),
        };

        VkImage       cImage{VK_NULL_HANDLE};
        VmaAllocation allocation{VK_NULL_HANDLE};

        DVRE_VK_CHECK(vmaCreateImage(
            allocator,
            &(const VkImageCreateInfo &)imageInfo,
            &allocationInfo,
            &cImage,
            &allocation,
            nullptr));

        return Allocation{
            .Image      = cImage,
            .Extent     = extent,
            .Format     = format,
            .Allocation = allocation,
            .Allocator  = allocator,
        };
    }

    Allocation Allocate(vk::MemoryPropertyFlags memoryFlags, vk::Format format, vk::ImageUsageFlags usageFlags, const vk::Extent2D &extent, const VmaAllocator &allocator) {
        vk::ImageCreateInfo imageInfo{
            {},
            vk::ImageType::e2D,
            format,
            vk::Extent3D{extent, 1u},
            1u,
            1u,
            vk::SampleCountFlagBits::e1,
            vk::ImageTiling::eOptimal,
            usageFlags,
        };
        VmaAllocationCreateInfo allocationInfo{
            .usage         = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
            .requiredFlags = VkMemoryPropertyFlags(memoryFlags),
        };

        VkImage       cImage{VK_NULL_HANDLE};
        VmaAllocation allocation{VK_NULL_HANDLE};

        DVRE_VK_CHECK(vmaCreateImage(
            allocator,
            &(const VkImageCreateInfo &)imageInfo,
            &allocationInfo,
            &cImage,
            &allocation,
            nullptr));

        return Allocation{
            .Image      = cImage,
            .Extent     = vk::Extent3D{extent, 1u},
            .Format     = format,
            .Allocation = allocation,
            .Allocator  = allocator,
        };
    }

    vk::ImageView CreateView(const Allocation &image, vk::ImageAspectFlags aspectFlags, const vk::Device &device) {
        vk::ImageViewCreateInfo viewInfo{
            {},
            image.Image,
            image.Extent.depth == 1
                ? vk::ImageViewType::e2D
                : vk::ImageViewType::e3D,
            image.Format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                aspectFlags,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            },
        };

        auto [result, view] = device.createImageView(viewInfo);
        DVRE_VK_CHECK(result);

        return view;
    }

    vk::ImageView CreateColorView(const Allocation &image, const vk::Device &device) {
        vk::ImageViewCreateInfo viewInfo{
            {},
            image.Image,
            image.Extent.depth == 1
                ? vk::ImageViewType::e2D
                : vk::ImageViewType::e3D,
            image.Format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eColor,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            },
        };

        auto [result, view] = device.createImageView(viewInfo);
        DVRE_VK_CHECK(result);

        return view;
    }

    vk::ImageView CreateDepthView(const Allocation &image, const vk::Device &device) {
        vk::ImageViewCreateInfo viewInfo{
            {},
            image.Image,
            image.Extent.depth == 1
                ? vk::ImageViewType::e2D
                : vk::ImageViewType::e3D,
            image.Format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eDepth,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            },
        };

        auto [result, view] = device.createImageView(viewInfo);
        DVRE_VK_CHECK(result);

        return view;
    }

    vk::ImageView CreateView3D(const vk::Image &image, vk::Format format, vk::ImageAspectFlags aspectFlags, const vk::Device &device) {
        vk::ImageViewCreateInfo viewInfo{
            {},
            image,
            vk::ImageViewType::e3D,
            format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                aspectFlags,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            },
        };

        auto [result, view] = device.createImageView(viewInfo);
        DVRE_VK_CHECK(result);

        return view;
    }

    vk::ImageView CreateColorView3D(const vk::Image &image, vk::Format format, const vk::Device &device) {
        vk::ImageViewCreateInfo viewInfo{
            {},
            image,
            vk::ImageViewType::e3D,
            format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eColor,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            },
        };

        auto [result, view] = device.createImageView(viewInfo);
        DVRE_VK_CHECK(result);

        return view;
    }

    vk::ImageView CreateDepthView3D(const vk::Image &image, vk::Format format, const vk::Device &device) {
        vk::ImageViewCreateInfo viewInfo{
            {},
            image,
            vk::ImageViewType::e3D,
            format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eDepth,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            },
        };

        auto [result, view] = device.createImageView(viewInfo);
        DVRE_VK_CHECK(result);

        return view;
    }

    vk::ImageView CreateView2D(const vk::Image &image, vk::Format format, vk::ImageAspectFlags aspectFlags, const vk::Device &device) {
        vk::ImageViewCreateInfo viewInfo{
            {},
            image,
            vk::ImageViewType::e2D,
            format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                aspectFlags,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            },
        };

        auto [result, view] = device.createImageView(viewInfo);
        DVRE_VK_CHECK(result);

        return view;
    }

    vk::ImageView CreateColorView2D(const vk::Image &image, vk::Format format, const vk::Device &device) {
        vk::ImageViewCreateInfo viewInfo{
            {},
            image,
            vk::ImageViewType::e2D,
            format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eColor,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            },
        };

        auto [result, view] = device.createImageView(viewInfo);
        DVRE_VK_CHECK(result);

        return view;
    }

    vk::ImageView CreateDepthView2D(const vk::Image &image, vk::Format format, const vk::Device &device) {
        vk::ImageViewCreateInfo viewInfo{
            {},
            image,
            vk::ImageViewType::e2D,
            format,
            vk::ComponentMapping{},
            vk::ImageSubresourceRange{
                vk::ImageAspectFlagBits::eDepth,
                0u,
                vk::RemainingMipLevels,
                0u,
                vk::RemainingArrayLayers,
            },
        };

        auto [result, view] = device.createImageView(viewInfo);
        DVRE_VK_CHECK(result);

        return view;
    }

}  // namespace vre::Vulkan::Image