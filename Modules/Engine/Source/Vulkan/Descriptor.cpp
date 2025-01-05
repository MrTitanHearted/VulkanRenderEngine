#include <VREngine/Vulkan/Descriptor.hpp>

namespace vre::Vulkan {
    namespace DescriptorLayout {
        void Builder::clear() {
            m_Bindings.clear();
        }

        Builder &Builder::setFlags(vk::DescriptorSetLayoutCreateFlags flags) {
            m_Flags = flags;
            return *this;
        }

        Builder &Builder::addBinding(std::uint32_t binding, vk::DescriptorType type, vk::ShaderStageFlags stageFlags) {
            m_Bindings.push_back(vk::DescriptorSetLayoutBinding{
                binding,
                type,
                stageFlags,
                {},
            });

            return *this;
        }

        Builder &Builder::addBinding(std::uint32_t binding, vk::DescriptorType type) {
            m_Bindings.push_back(vk::DescriptorSetLayoutBinding{
                binding,
                type,
                {},
                {},
            });

            return *this;
        }

        vk::DescriptorSetLayout Builder::build(
            vk::ShaderStageFlags stageFlags,
            const vk::Device    &device) {
            for (vk::DescriptorSetLayoutBinding &binding : m_Bindings) {
                binding.setStageFlags(stageFlags);
            }

            auto [result, layout] = device.createDescriptorSetLayout(vk::DescriptorSetLayoutCreateInfo{
                m_Flags,
                m_Bindings,
            });

            DVRE_VK_CHECK(result);

            return layout;
        }

        vk::DescriptorSetLayout Builder::build(const vk::Device &device) {
            auto [result, layout] = device.createDescriptorSetLayout(vk::DescriptorSetLayoutCreateInfo{
                m_Flags,
                m_Bindings,
            });

            DVRE_VK_CHECK(result);

            return layout;
        }

        vk::DescriptorSetLayout Create(
            vk::DescriptorSetLayoutCreateFlags flags,
            const vk::ArrayProxy<Binding>     &bindings,
            vk::ShaderStageFlags               stageFlags,
            const vk::Device                  &device) {
            std::vector<vk::DescriptorSetLayoutBinding> vkBindings{};
            vkBindings.reserve(bindings.size());

            for (const Binding &binding : bindings)
                vkBindings.push_back(vk::DescriptorSetLayoutBinding{
                    binding.Binding,
                    binding.Type,
                    stageFlags,
                    {},
                });

            auto [result, layout] = device.createDescriptorSetLayout(vk::DescriptorSetLayoutCreateInfo{
                flags,
                vkBindings,
            });

            DVRE_VK_CHECK(result);

            return layout;
        }

        vk::DescriptorSetLayout Create(
            vk::DescriptorSetLayoutCreateFlags flags,
            const vk::ArrayProxy<Binding>     &bindings,
            const vk::Device                  &device) {
            std::vector<vk::DescriptorSetLayoutBinding> vkBindings{};
            vkBindings.reserve(bindings.size());

            for (const Binding &binding : bindings)
                vkBindings.push_back(vk::DescriptorSetLayoutBinding{
                    binding.Binding,
                    binding.Type,
                    binding.StageFlags,
                    {},
                });

            auto [result, layout] = device.createDescriptorSetLayout(vk::DescriptorSetLayoutCreateInfo{
                flags,
                vkBindings,
            });

            DVRE_VK_CHECK(result);

            return layout;
        }

        vk::DescriptorSetLayout Create(
            const vk::ArrayProxy<Binding> &bindings,
            vk::ShaderStageFlags           stageFlags,
            const vk::Device              &device) {
            std::vector<vk::DescriptorSetLayoutBinding> vkBindings{};
            vkBindings.reserve(bindings.size());

            for (const Binding &binding : bindings)
                vkBindings.push_back(vk::DescriptorSetLayoutBinding{
                    binding.Binding,
                    binding.Type,
                    stageFlags,
                    {},
                });

            auto [result, layout] = device.createDescriptorSetLayout(vk::DescriptorSetLayoutCreateInfo{
                {},
                vkBindings,
            });

            DVRE_VK_CHECK(result);

            return layout;
        }

        vk::DescriptorSetLayout Create(
            const vk::ArrayProxy<Binding> &bindings,
            const vk::Device              &device) {
            std::vector<vk::DescriptorSetLayoutBinding> vkBindings{};
            vkBindings.reserve(bindings.size());

            for (const Binding &binding : bindings)
                vkBindings.push_back(vk::DescriptorSetLayoutBinding{
                    binding.Binding,
                    binding.Type,
                    binding.StageFlags,
                    {},
                });

            auto [result, layout] = device.createDescriptorSetLayout(vk::DescriptorSetLayoutCreateInfo{
                {},
                vkBindings,
            });

            DVRE_VK_CHECK(result);

            return layout;
        }

    }  // namespace DescriptorLayout

    namespace DescriptorPool {
        vk::DescriptorPool Create(
            vk::DescriptorPoolCreateFlags                 flags,
            std::uint32_t                                 maxSets,
            const vk::ArrayProxy<vk::DescriptorPoolSize> &poolSizes,
            const vk::Device                             &device) {
            auto [result, pool] = device.createDescriptorPool(vk::DescriptorPoolCreateInfo{
                flags,
                maxSets,
                poolSizes,
            });

            DVRE_VK_CHECK(result);

            return pool;
        }

        vk::DescriptorPool Create(
            std::uint32_t                                 maxSets,
            const vk::ArrayProxy<vk::DescriptorPoolSize> &poolSizes,
            const vk::Device                             &device) {
            auto [result, pool] = device.createDescriptorPool(vk::DescriptorPoolCreateInfo{
                {},
                maxSets,
                poolSizes,
            });

            DVRE_VK_CHECK(result);

            return pool;
        }

        vk::DescriptorPool CreateFreeDescriptorSets(
            std::uint32_t                                 maxSets,
            const vk::ArrayProxy<vk::DescriptorPoolSize> &poolSizes,
            const vk::Device                             &device) {
            auto [result, pool] = device.createDescriptorPool(vk::DescriptorPoolCreateInfo{
                vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
                maxSets,
                poolSizes,
            });

            DVRE_VK_CHECK(result);

            return pool;
        }

        vk::DescriptorPool Create(
            vk::DescriptorPoolCreateFlags        flags,
            std::uint32_t                        maxSets,
            const vk::ArrayProxy<PoolSizeRatio> &poolSizeRatios,
            const vk::Device                    &device) {
            std::vector<vk::DescriptorPoolSize> vkPoolSizes{};
            vkPoolSizes.reserve(poolSizeRatios.size());

            for (const PoolSizeRatio &r : poolSizeRatios)
                vkPoolSizes.push_back(vk::DescriptorPoolSize{
                    r.Type,
                    std::uint32_t(float(maxSets) * r.Ratio),
                });

            auto [result, pool] = device.createDescriptorPool(vk::DescriptorPoolCreateInfo{
                flags,
                maxSets,
                vkPoolSizes,
            });

            DVRE_VK_CHECK(result);

            return pool;
        }

        vk::DescriptorPool Create(
            std::uint32_t                        maxSets,
            const vk::ArrayProxy<PoolSizeRatio> &poolSizeRatios,
            const vk::Device                    &device) {
            std::vector<vk::DescriptorPoolSize> vkPoolSizes{};
            vkPoolSizes.reserve(poolSizeRatios.size());

            for (const PoolSizeRatio &r : poolSizeRatios)
                vkPoolSizes.push_back(vk::DescriptorPoolSize{
                    r.Type,
                    std::uint32_t(float(maxSets) * r.Ratio),
                });

            auto [result, pool] = device.createDescriptorPool(vk::DescriptorPoolCreateInfo{
                {},
                maxSets,
                vkPoolSizes,
            });

            DVRE_VK_CHECK(result);

            return pool;
        }

        vk::DescriptorPool CreateFreeDescriptorSets(
            std::uint32_t                        maxSets,
            const vk::ArrayProxy<PoolSizeRatio> &poolSizeRatios,
            const vk::Device                    &device) {
            std::vector<vk::DescriptorPoolSize> vkPoolSizes{};
            vkPoolSizes.reserve(poolSizeRatios.size());

            for (const PoolSizeRatio &r : poolSizeRatios)
                vkPoolSizes.push_back(vk::DescriptorPoolSize{
                    r.Type,
                    std::uint32_t(float(maxSets) * r.Ratio),
                });

            auto [result, pool] = device.createDescriptorPool(vk::DescriptorPoolCreateInfo{
                vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
                maxSets,
                vkPoolSizes,
            });

            DVRE_VK_CHECK(result);

            return pool;
        }

        void Clear(const vk::DescriptorPool &pool, const vk::Device &device) {
            device.resetDescriptorPool(pool);
        }
    }  // namespace DescriptorPool

    namespace DescriptorSet {
        void Binder::clear() {
            m_BufferBindings.clear();
            m_ImageBindings.clear();
            m_BufferInfos.clear();
            m_ImageInfos.clear();
        }

        void Binder::bind(const vk::DescriptorSet &set, const vk::Device &device) {
            std::vector<vk::WriteDescriptorSet> writes{};
            writes.reserve(m_BufferBindings.size() + m_ImageBindings.size());

            for (const Binding &binding : m_ImageBindings)
                writes.push_back(vk::WriteDescriptorSet{
                    set,
                    binding.Binding,
                    0u,
                    binding.Type,
                    {m_ImageInfos[binding.InfoIndex]},
                    {},
                    {},
                });

            for (const Binding &binding : m_BufferBindings)
                writes.push_back(vk::WriteDescriptorSet{
                    set,
                    binding.Binding,
                    0u,
                    binding.Type,
                    {},
                    {m_BufferInfos[binding.InfoIndex]},
                    {},
                });

            device.updateDescriptorSets(writes, {});
        }

        void Binder::bind(const vk::Device &device) {
            std::vector<vk::WriteDescriptorSet> writes{};
            writes.reserve(m_BufferBindings.size() + m_ImageBindings.size());

            for (const Binding &binding : m_ImageBindings) {
                DVRE_ASSERT(!!binding.Set, "vk::DescriptorSet should be provided!");
                writes.push_back(vk::WriteDescriptorSet{
                    binding.Set,
                    binding.Binding,
                    0u,
                    binding.Type,
                    {m_ImageInfos[binding.InfoIndex]},
                    {},
                    {},
                });
            }

            for (const Binding &binding : m_BufferBindings) {
                DVRE_ASSERT(!!binding.Set, "vk::DescriptorSet should be provided!");
                writes.push_back(vk::WriteDescriptorSet{
                    binding.Set,
                    binding.Binding,
                    0u,
                    binding.Type,
                    {},
                    {m_BufferInfos[binding.InfoIndex]},
                    {},
                });
            }

            device.updateDescriptorSets(writes, {});
        }

        Binder &Binder::addUniformBuffer(
            const vk::DescriptorSet  &set,
            std::uint32_t             binding,
            const Buffer::Allocation &buffer) {
            m_BufferBindings.push_back(Binding{
                .Set       = set,
                .Binding   = binding,
                .Type      = vk::DescriptorType::eUniformBuffer,
                .InfoIndex = std::uint32_t(m_BufferInfos.size()),
            });
            m_BufferInfos.push_back(vk::DescriptorBufferInfo{
                buffer.Buffer,
                0Ui64,
                buffer.Size,
            });
            return *this;
        }

        Binder &Binder::addStorageBuffer(
            const vk::DescriptorSet  &set,
            std::uint32_t             binding,
            const Buffer::Allocation &buffer) {
            m_BufferBindings.push_back(Binding{
                .Set       = set,
                .Binding   = binding,
                .Type      = vk::DescriptorType::eStorageBuffer,
                .InfoIndex = std::uint32_t(m_BufferInfos.size()),
            });
            m_BufferInfos.push_back(vk::DescriptorBufferInfo{
                buffer.Buffer,
                0Ui64,
                buffer.Size,
            });
            return *this;
        }

        Binder &Binder::addUniformBuffer(
            const vk::DescriptorSet &set,
            std::uint32_t            binding,
            const vk::Buffer        &buffer,
            std::uint64_t            size) {
            m_BufferBindings.push_back(Binding{
                .Set       = set,
                .Binding   = binding,
                .Type      = vk::DescriptorType::eUniformBuffer,
                .InfoIndex = std::uint32_t(m_BufferInfos.size()),
            });
            m_BufferInfos.push_back(vk::DescriptorBufferInfo{
                buffer,
                0Ui64,
                size,
            });
            return *this;
        }

        Binder &Binder::addStorageBuffer(
            const vk::DescriptorSet &set,
            std::uint32_t            binding,
            const vk::Buffer        &buffer,
            std::uint64_t            size) {
            m_BufferBindings.push_back(Binding{
                .Set       = set,
                .Binding   = binding,
                .Type      = vk::DescriptorType::eStorageBuffer,
                .InfoIndex = std::uint32_t(m_BufferInfos.size()),
            });
            m_BufferInfos.push_back(vk::DescriptorBufferInfo{
                buffer,
                0Ui64,
                size,
            });
            return *this;
        }

        Binder &Binder::addSampledImage(
            const vk::DescriptorSet &set,
            std::uint32_t            binding,
            const vk::ImageView     &view,
            vk::ImageLayout          layout) {
            m_ImageBindings.push_back(Binding{
                .Set       = set,
                .Binding   = binding,
                .Type      = vk::DescriptorType::eSampledImage,
                .InfoIndex = std::uint32_t(m_ImageInfos.size()),
            });
            m_ImageInfos.push_back(vk::DescriptorImageInfo{
                VK_NULL_HANDLE,
                view,
                layout,
            });
            return *this;
        }

        Binder &Binder::addStorageImage(
            const vk::DescriptorSet &set,
            std::uint32_t            binding,
            const vk::ImageView     &view,
            vk::ImageLayout          layout) {
            m_ImageBindings.push_back(Binding{
                .Set       = set,
                .Binding   = binding,
                .Type      = vk::DescriptorType::eStorageImage,
                .InfoIndex = std::uint32_t(m_ImageInfos.size()),
            });
            m_ImageInfos.push_back(vk::DescriptorImageInfo{
                VK_NULL_HANDLE,
                view,
                layout,
            });
            return *this;
        }

        Binder &Binder::addInputAttachment(
            const vk::DescriptorSet &set,
            std::uint32_t            binding,
            const vk::ImageView     &view,
            vk::ImageLayout          layout) {
            m_ImageBindings.push_back(Binding{
                .Set       = set,
                .Binding   = binding,
                .Type      = vk::DescriptorType::eInputAttachment,
                .InfoIndex = std::uint32_t(m_ImageInfos.size()),
            });
            m_ImageInfos.push_back(vk::DescriptorImageInfo{
                VK_NULL_HANDLE,
                view,
                layout,
            });
            return *this;
        }

        Binder &Binder::addSampler(
            const vk::DescriptorSet &set,
            std::uint32_t            binding,
            const vk::Sampler       &sampler) {
            m_ImageBindings.push_back(Binding{
                .Set       = set,
                .Binding   = binding,
                .Type      = vk::DescriptorType::eSampler,
                .InfoIndex = std::uint32_t(m_ImageInfos.size()),
            });
            m_ImageInfos.push_back(vk::DescriptorImageInfo{sampler});
            return *this;
        }

        Binder &Binder::addCombinedImageSampler(
            const vk::DescriptorSet &set,
            std::uint32_t            binding,
            const vk::ImageView     &view,
            vk::ImageLayout          layout,
            const vk::Sampler       &sampler) {
            m_ImageBindings.push_back(Binding{
                .Set       = set,
                .Binding   = binding,
                .Type      = vk::DescriptorType::eCombinedImageSampler,
                .InfoIndex = std::uint32_t(m_ImageInfos.size()),
            });
            m_ImageInfos.push_back(vk::DescriptorImageInfo{
                sampler,
                view,
                layout,
            });
            return *this;
        }

        Binder &Binder::addUniformBuffer(
            std::uint32_t             binding,
            const Buffer::Allocation &buffer) {
            m_BufferBindings.push_back(Binding{
                .Binding   = binding,
                .Type      = vk::DescriptorType::eUniformBuffer,
                .InfoIndex = std::uint32_t(m_BufferInfos.size()),
            });
            m_BufferInfos.push_back(vk::DescriptorBufferInfo{
                buffer.Buffer,
                0Ui64,
                buffer.Size,
            });
            return *this;
        }

        Binder &Binder::addStorageBuffer(
            std::uint32_t             binding,
            const Buffer::Allocation &buffer) {
            m_BufferBindings.push_back(Binding{
                .Binding   = binding,
                .Type      = vk::DescriptorType::eStorageBuffer,
                .InfoIndex = std::uint32_t(m_BufferInfos.size()),
            });
            m_BufferInfos.push_back(vk::DescriptorBufferInfo{
                buffer.Buffer,
                0Ui64,
                buffer.Size,
            });
            return *this;
        }

        Binder &Binder::addUniformBuffer(
            std::uint32_t     binding,
            const vk::Buffer &buffer,
            std::uint64_t     size) {
            m_BufferBindings.push_back(Binding{
                .Binding   = binding,
                .Type      = vk::DescriptorType::eUniformBuffer,
                .InfoIndex = std::uint32_t(m_BufferInfos.size()),
            });
            m_BufferInfos.push_back(vk::DescriptorBufferInfo{
                buffer,
                0Ui64,
                size,
            });
            return *this;
        }

        Binder &Binder::addStorageBuffer(
            std::uint32_t     binding,
            const vk::Buffer &buffer,
            std::uint64_t     size) {
            m_BufferBindings.push_back(Binding{
                .Binding   = binding,
                .Type      = vk::DescriptorType::eStorageBuffer,
                .InfoIndex = std::uint32_t(m_BufferInfos.size()),
            });
            m_BufferInfos.push_back(vk::DescriptorBufferInfo{
                buffer,
                0Ui64,
                size,
            });
            return *this;
        }

        Binder &Binder::addSampledImage(
            std::uint32_t        binding,
            const vk::ImageView &view,
            vk::ImageLayout      layout) {
            m_ImageBindings.push_back(Binding{
                .Binding   = binding,
                .Type      = vk::DescriptorType::eSampledImage,
                .InfoIndex = std::uint32_t(m_ImageInfos.size()),
            });
            m_ImageInfos.push_back(vk::DescriptorImageInfo{
                VK_NULL_HANDLE,
                view,
                layout,
            });
            return *this;
        }

        Binder &Binder::addStorageImage(
            std::uint32_t        binding,
            const vk::ImageView &view,
            vk::ImageLayout      layout) {
            m_ImageBindings.push_back(Binding{
                .Binding   = binding,
                .Type      = vk::DescriptorType::eStorageImage,
                .InfoIndex = std::uint32_t(m_ImageInfos.size()),
            });
            m_ImageInfos.push_back(vk::DescriptorImageInfo{
                VK_NULL_HANDLE,
                view,
                layout,
            });
            return *this;
        }

        Binder &Binder::addInputAttachment(
            std::uint32_t        binding,
            const vk::ImageView &view,
            vk::ImageLayout      layout) {
            m_ImageBindings.push_back(Binding{
                .Binding   = binding,
                .Type      = vk::DescriptorType::eInputAttachment,
                .InfoIndex = std::uint32_t(m_ImageInfos.size()),
            });
            m_ImageInfos.push_back(vk::DescriptorImageInfo{
                VK_NULL_HANDLE,
                view,
                layout,
            });
            return *this;
        }

        Binder &Binder::addSampler(
            std::uint32_t      binding,
            const vk::Sampler &sampler) {
            m_ImageBindings.push_back(Binding{
                .Binding   = binding,
                .Type      = vk::DescriptorType::eSampler,
                .InfoIndex = std::uint32_t(m_ImageInfos.size()),
            });
            m_ImageInfos.push_back(vk::DescriptorImageInfo{sampler});
            return *this;
        }

        Binder &Binder::addCombinedImageSampler(
            std::uint32_t        binding,
            const vk::ImageView &view,
            vk::ImageLayout      layout,
            const vk::Sampler   &sampler) {
            m_ImageBindings.push_back(Binding{
                .Binding   = binding,
                .Type      = vk::DescriptorType::eCombinedImageSampler,
                .InfoIndex = std::uint32_t(m_ImageInfos.size()),
            });
            m_ImageInfos.push_back(vk::DescriptorImageInfo{
                sampler,
                view,
                layout,
            });
            return *this;
        }

        vk::DescriptorSet Allocate(
            const vk::DescriptorPool      &pool,
            const vk::DescriptorSetLayout &layout,
            const vk::Device              &device) {
            auto [result, sets] = device.allocateDescriptorSets(vk::DescriptorSetAllocateInfo{
                pool,
                {layout},
            });

            DVRE_VK_CHECK(result);
            DVRE_ASSERT(sets.size() == 1);

            return sets[0];
        }

        std::vector<vk::DescriptorSet> Allocate(
            const vk::DescriptorPool                      &pool,
            const vk::ArrayProxy<vk::DescriptorSetLayout> &layouts,
            const vk::Device                              &device) {
            auto [result, sets] = device.allocateDescriptorSets(vk::DescriptorSetAllocateInfo{
                pool,
                layouts,
            });

            DVRE_VK_CHECK(result);
            DVRE_ASSERT(sets.size() == layouts.size());

            return sets;
        }
    }  // namespace DescriptorSet
}  // namespace vre::Vulkan