#pragma once

#include <VREngine/Core.hpp>
#include <VREngine/Vulkan/Types.hpp>

namespace vre::Vulkan {
    namespace DescriptorLayout {
        class Builder {
           public:
            Builder()  = default;
            ~Builder() = default;

            void clear();

            Builder &setFlags(vk::DescriptorSetLayoutCreateFlags flags);
            Builder &addBinding(std::uint32_t binding, vk::DescriptorType type, vk::ShaderStageFlags stageFlags);
            Builder &addBinding(std::uint32_t binding, vk::DescriptorType type);

            vk::DescriptorSetLayout build(
                vk::ShaderStageFlags stageFlags,
                const vk::Device    &device);
            vk::DescriptorSetLayout build(const vk::Device &device);

           private:
            std::vector<vk::DescriptorSetLayoutBinding> m_Bindings;
            vk::DescriptorSetLayoutCreateFlags          m_Flags;
        };

        vk::DescriptorSetLayout Create(
            vk::DescriptorSetLayoutCreateFlags flags,
            const vk::ArrayProxy<Binding>     &bindings,
            vk::ShaderStageFlags               stageFlags,
            const vk::Device                  &device);
        vk::DescriptorSetLayout Create(
            vk::DescriptorSetLayoutCreateFlags flags,
            const vk::ArrayProxy<Binding>     &bindings,
            const vk::Device                  &device);
        vk::DescriptorSetLayout Create(
            const vk::ArrayProxy<Binding> &bindings,
            vk::ShaderStageFlags           stageFlags,
            const vk::Device              &device);
        vk::DescriptorSetLayout Create(
            const vk::ArrayProxy<Binding> &bindings,
            const vk::Device              &device);
    }  // namespace DescriptorLayout

    namespace DescriptorPool {
        vk::DescriptorPool Create(
            vk::DescriptorPoolCreateFlags                 flags,
            std::uint32_t                                 maxSets,
            const vk::ArrayProxy<vk::DescriptorPoolSize> &poolSizes,
            const vk::Device                             &device);
        vk::DescriptorPool Create(
            std::uint32_t                                 maxSets,
            const vk::ArrayProxy<vk::DescriptorPoolSize> &poolSizes,
            const vk::Device                             &device);
        vk::DescriptorPool CreateFreeDescriptorSets(
            std::uint32_t                                 maxSets,
            const vk::ArrayProxy<vk::DescriptorPoolSize> &poolSizes,
            const vk::Device                             &device);

        vk::DescriptorPool Create(
            vk::DescriptorPoolCreateFlags        flags,
            std::uint32_t                        maxSets,
            const vk::ArrayProxy<PoolSizeRatio> &poolSizeRatios,
            const vk::Device                    &device);
        vk::DescriptorPool Create(
            std::uint32_t                        maxSets,
            const vk::ArrayProxy<PoolSizeRatio> &poolSizeRatios,
            const vk::Device                    &device);
        vk::DescriptorPool CreateFreeDescriptorSets(
            std::uint32_t                        maxSets,
            const vk::ArrayProxy<PoolSizeRatio> &poolSizeRatios,
            const vk::Device                    &device);

        void Clear(const vk::DescriptorPool &pool, const vk::Device &device);
    }  // namespace DescriptorPool

    namespace DescriptorSet {
        class Binder {
           public:
            Binder()  = default;
            ~Binder() = default;

            void clear();
            void bind(const vk::DescriptorSet &set, const vk::Device &device);
            void bind(const vk::Device &device);

            Binder &addUniformBuffer(
                const vk::DescriptorSet  &set,
                std::uint32_t             binding,
                const Buffer::Allocation &buffer);
            Binder &addStorageBuffer(
                const vk::DescriptorSet  &set,
                std::uint32_t             binding,
                const Buffer::Allocation &buffer);
            Binder &addUniformBuffer(
                const vk::DescriptorSet &set,
                std::uint32_t            binding,
                const vk::Buffer        &buffer,
                std::uint64_t            size);
            Binder &addStorageBuffer(
                const vk::DescriptorSet &set,
                std::uint32_t            binding,
                const vk::Buffer        &buffer,
                std::uint64_t            size);
            Binder &addSampledImage(
                const vk::DescriptorSet &set,
                std::uint32_t            binding,
                const vk::ImageView     &view,
                vk::ImageLayout          layout);
            Binder &addStorageImage(
                const vk::DescriptorSet &set,
                std::uint32_t            binding,
                const vk::ImageView     &view,
                vk::ImageLayout          layout);
            Binder &addInputAttachment(
                const vk::DescriptorSet &set,
                std::uint32_t            binding,
                const vk::ImageView     &view,
                vk::ImageLayout          layout);
            Binder &addSampler(
                const vk::DescriptorSet &set,
                std::uint32_t            binding,
                const vk::Sampler       &sampler);
            Binder &addCombinedImageSampler(
                const vk::DescriptorSet &set,
                std::uint32_t            binding,
                const vk::ImageView     &view,
                vk::ImageLayout          layout,
                const vk::Sampler       &sampler);

            Binder &addUniformBuffer(std::uint32_t binding, const Buffer::Allocation &buffer);
            Binder &addStorageBuffer(std::uint32_t binding, const Buffer::Allocation &buffer);
            Binder &addUniformBuffer(
                std::uint32_t     binding,
                const vk::Buffer &buffer,
                std::uint64_t     size);
            Binder &addStorageBuffer(
                std::uint32_t     binding,
                const vk::Buffer &buffer,
                std::uint64_t     size);
            Binder &addSampledImage(
                std::uint32_t        binding,
                const vk::ImageView &view,
                vk::ImageLayout      layout);
            Binder &addStorageImage(
                std::uint32_t        binding,
                const vk::ImageView &view,
                vk::ImageLayout      layout);
            Binder &addInputAttachment(
                std::uint32_t        binding,
                const vk::ImageView &view,
                vk::ImageLayout      layout);
            Binder &addSampler(
                std::uint32_t      binding,
                const vk::Sampler &sampler);
            Binder &addCombinedImageSampler(
                std::uint32_t        binding,
                const vk::ImageView &view,
                vk::ImageLayout      layout,
                const vk::Sampler   &sampler);

           private:
            struct Binding {
                vk::DescriptorSet  Set;
                std::uint32_t      Binding;
                vk::DescriptorType Type;
                std::uint32_t      InfoIndex;
            };

           private:
            std::vector<vk::DescriptorBufferInfo> m_BufferInfos;
            std::vector<vk::DescriptorImageInfo>  m_ImageInfos;

            std::vector<Binding> m_BufferBindings;
            std::vector<Binding> m_ImageBindings;
        };

        vk::DescriptorSet Allocate(
            const vk::DescriptorPool      &pool,
            const vk::DescriptorSetLayout &layout,
            const vk::Device              &device);

        std::vector<vk::DescriptorSet> Allocate(
            const vk::DescriptorPool                      &pool,
            const vk::ArrayProxy<vk::DescriptorSetLayout> &layouts,
            const vk::Device                              &device);
    }  // namespace DescriptorSet
}  // namespace vre::Vulkan