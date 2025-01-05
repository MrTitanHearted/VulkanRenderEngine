#include <VREngine/Vulkan/Pipeline.hpp>

namespace vre::Vulkan {
    namespace PipelineLayout {
        vk::PipelineLayout Create(
            const vk::ArrayProxy<vk::DescriptorSetLayout> &setLayouts,
            const vk::ArrayProxy<vk::PushConstantRange>   &pushConstants,
            const vk::Device                              &device) {
            auto [result, layout] = device.createPipelineLayout(vk::PipelineLayoutCreateInfo{
                {},
                setLayouts,
                pushConstants,
            });

            DVRE_VK_CHECK(result);

            return layout;
        }

        vk::PipelineLayout Create(
            const vk::ArrayProxy<vk::DescriptorSetLayout> &setLayouts,
            const vk::Device                              &device) {
            auto [result, layout] = device.createPipelineLayout(vk::PipelineLayoutCreateInfo{
                {},
                setLayouts,
                {},
            });

            DVRE_VK_CHECK(result);

            return layout;
        }
    }  // namespace PipelineLayout

    namespace ComputePipeline {
        vk::Pipeline Create(
            const std::string        &entry,
            const vk::ShaderModule   &module,
            const vk::PipelineLayout &layout,
            const vk::Device         &device) {
            vk::PipelineShaderStageCreateInfo shaderInfo{
                {},
                vk::ShaderStageFlagBits::eCompute,
                module,
                entry.c_str(),
            };

            auto [result, pipeline] = device.createComputePipeline(
                VK_NULL_HANDLE,
                vk::ComputePipelineCreateInfo{
                    {},
                    shaderInfo,
                    layout,
                });

            DVRE_VK_CHECK(result);

            return pipeline;
        }
    }  // namespace ComputePipeline

    namespace GraphicsPipeline {
        Builder::Builder() { clear(); }

        void Builder::clear() {
            m_InputAssembly = vk::PipelineInputAssemblyStateCreateInfo{
                {},
                vk::PrimitiveTopology::eTriangleList,
                vk::False,
            };
            m_Rasterizer = vk::PipelineRasterizationStateCreateInfo{
                {},
                vk::False,
                vk::False,
                vk::PolygonMode::eFill,
                vk::CullModeFlagBits::eNone,
                vk::FrontFace::eCounterClockwise,
                vk::False,
                0.0f,
                0.0f,
                0.0f,
                1.0f,
            };
            m_ColorBlendAttachment = vk::PipelineColorBlendAttachmentState{};
            m_Multisampling        = vk::PipelineMultisampleStateCreateInfo{};
            m_DepthStencil         = vk::PipelineDepthStencilStateCreateInfo{};
            m_RenderingInfo        = vk::PipelineRenderingCreateInfo{};
            m_ShaderStages.clear();
            m_VertexInputAttributeDescriptions.clear();
            m_VertexInputBindingDescriptions.clear();
        }

        Builder &Builder::setShaders(const std::vector<Shader::StageInfo> &shaders) {
            m_ShaderStages.insert(m_ShaderStages.end(), shaders.begin(), shaders.end());
            return *this;
        }

        Builder &Builder::setVertexShader(const std::string &entry, const vk::ShaderModule &shader) {
            m_ShaderStages.push_back(Shader::StageInfo{
                .Module     = shader,
                .StageFlags = vk::ShaderStageFlagBits::eVertex,
                .Entry      = entry,
            });
            return *this;
        }

        Builder &Builder::setFragmentShader(const std::string &entry, const vk::ShaderModule &shader) {
            m_ShaderStages.push_back(Shader::StageInfo{
                .Module     = shader,
                .StageFlags = vk::ShaderStageFlagBits::eFragment,
                .Entry      = entry,
            });
            return *this;
        }

        Builder &Builder::setVertexLayouts(const std::vector<VertexLayout> &layouts) {
            for (const VertexLayout &layout : layouts) {
                m_VertexInputBindingDescriptions.push_back(layout.Binding);
                m_VertexInputAttributeDescriptions.insert(
                    m_VertexInputAttributeDescriptions.end(),
                    layout.Attributes.begin(),
                    layout.Attributes.end());
            }
            return *this;
        }

        Builder &Builder::setVertexLayout(const VertexLayout &layout) {
            m_VertexInputBindingDescriptions.push_back(layout.Binding);
            m_VertexInputAttributeDescriptions.insert(
                m_VertexInputAttributeDescriptions.end(),
                layout.Attributes.begin(),
                layout.Attributes.end());
            return *this;
        }

        Builder &Builder::setInputTopology(vk::PrimitiveTopology topology) {
            m_InputAssembly.setTopology(topology);
            return *this;
        }

        Builder &Builder::setPolygonMode(vk::PolygonMode mode) {
            m_Rasterizer.setPolygonMode(mode);
            return *this;
        }

        Builder &Builder::setCullMode(vk::CullModeFlags cullMode, vk::FrontFace frontFace) {
            m_Rasterizer
                .setCullMode(cullMode)
                .setFrontFace(frontFace);
            return *this;
        }

        Builder &Builder::setColorAttachmentFormats(const std::vector<vk::Format> &formats) {
            m_ColorAttachmentFormats.insert(
                m_ColorAttachmentFormats.end(),
                formats.begin(),
                formats.end());
            return *this;
        }

        Builder &Builder::setColorAttachmentFormat(vk::Format format) {
            m_ColorAttachmentFormats.push_back(format);
            return *this;
        }

        Builder &Builder::setDepthFormat(vk::Format format) {
            m_RenderingInfo.setDepthAttachmentFormat(format);
            return *this;
        }

        Builder &Builder::setDepthTest(bool depthWrite, vk::CompareOp op) {
            m_DepthStencil
                .setDepthTestEnable(vk::True)
                .setDepthWriteEnable(depthWrite ? vk::True : vk::False)
                .setDepthCompareOp(op)
                .setDepthBoundsTestEnable(vk::False)
                .setStencilTestEnable(vk::False)
                .setFront(vk::StencilOpState{})
                .setBack(vk::StencilOpState{})
                .setMinDepthBounds(0.0f)
                .setMaxDepthBounds(1.0f);
            return *this;
        }

        Builder &Builder::setBlendingAdditive() {
            m_ColorBlendAttachment
                .setColorWriteMask(vk::ColorComponentFlagBits::eR |
                                   vk::ColorComponentFlagBits::eG |
                                   vk::ColorComponentFlagBits::eB |
                                   vk::ColorComponentFlagBits::eA)
                .setBlendEnable(vk::True)
                .setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
                .setDstColorBlendFactor(vk::BlendFactor::eOne)
                .setColorBlendOp(vk::BlendOp::eAdd)
                .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
                .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
                .setAlphaBlendOp(vk::BlendOp::eAdd);
            return *this;
        }

        Builder &Builder::setBlendingAlphaBlended() {
            m_ColorBlendAttachment
                .setColorWriteMask(vk::ColorComponentFlagBits::eR |
                                   vk::ColorComponentFlagBits::eG |
                                   vk::ColorComponentFlagBits::eB |
                                   vk::ColorComponentFlagBits::eA)
                .setBlendEnable(vk::True)
                .setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
                .setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
                .setColorBlendOp(vk::BlendOp::eAdd)
                .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
                .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
                .setAlphaBlendOp(vk::BlendOp::eAdd);
            return *this;
        }

        Builder &Builder::setNoMultisampling() {
            m_Multisampling
                .setSampleShadingEnable(vk::False)
                .setMinSampleShading(1.0f)
                .setRasterizationSamples(vk::SampleCountFlagBits::e1)
                .setAlphaToCoverageEnable(vk::False)
                .setAlphaToOneEnable(vk::False);
            return *this;
        }

        Builder &Builder::setNoBlending() {
            m_ColorBlendAttachment
                .setColorWriteMask(vk::ColorComponentFlagBits::eR |
                                   vk::ColorComponentFlagBits::eG |
                                   vk::ColorComponentFlagBits::eB |
                                   vk::ColorComponentFlagBits::eA)
                .setBlendEnable(vk::False);
            return *this;
        }

        Builder &Builder::setNoDepthTest() {
            m_DepthStencil
                .setDepthTestEnable(vk::False)
                .setDepthWriteEnable(vk::False)
                .setDepthCompareOp(vk::CompareOp::eNever)
                .setDepthBoundsTestEnable(vk::False)
                .setStencilTestEnable(vk::False)
                .setFront(vk::StencilOpState{})
                .setBack(vk::StencilOpState{})
                .setMinDepthBounds(0.0f)
                .setMaxDepthBounds(1.0f);
            return *this;
        }

        vk::Pipeline Builder::build(const vk::PipelineLayout &layout, const vk::Device &device) {
            m_RenderingInfo.setColorAttachmentFormats(m_ColorAttachmentFormats);
            vk::Viewport viewport{};
            vk::Rect2D   scissor{};

            vk::PipelineViewportStateCreateInfo viewportState{
                {},
                {viewport},
                {scissor},
            };

            vk::DynamicState dynamicStates[]{vk::DynamicState::eViewport, vk::DynamicState::eScissor};

            vk::PipelineDynamicStateCreateInfo dynamicStateInfo{
                {},
                dynamicStates,
            };

            vk::PipelineColorBlendStateCreateInfo colorBlendState{
                {},
                vk::False,
                vk::LogicOp::eCopy,
                {m_ColorBlendAttachment},
            };

            vk::PipelineVertexInputStateCreateInfo vertexInputInfo{
                {},
                m_VertexInputBindingDescriptions,
                m_VertexInputAttributeDescriptions,
            };

            std::vector<vk::PipelineShaderStageCreateInfo> shaderStages{};
            shaderStages.reserve(m_ShaderStages.size());
            for (const Shader::StageInfo &info : m_ShaderStages)
                shaderStages.push_back(vk::PipelineShaderStageCreateInfo{
                    {},
                    static_cast<vk::ShaderStageFlagBits>(std::uint32_t(info.StageFlags)),
                    info.Module,
                    info.Entry.c_str(),
                });

            vk::GraphicsPipelineCreateInfo pipelineInfo{
                {},
                shaderStages,
                &vertexInputInfo,
                &m_InputAssembly,
                nullptr,
                &viewportState,
                &m_Rasterizer,
                &m_Multisampling,
                &m_DepthStencil,
                &colorBlendState,
                &dynamicStateInfo,
                layout,
                VK_NULL_HANDLE,
                0u,
                VK_NULL_HANDLE,
                0,
                &m_RenderingInfo,
            };

            auto [result, pipeline] = device.createGraphicsPipeline({}, pipelineInfo);

            DVRE_VK_CHECK(result);

            return pipeline;
        }
    }  // namespace GraphicsPipeline
}  // namespace vre::Vulkan