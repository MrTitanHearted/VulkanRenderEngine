#pragma once

#include <VREngine/Core.hpp>
#include <VREngine/Vulkan/Types.hpp>

namespace vre::Vulkan {
    namespace PipelineLayout {
        vk::PipelineLayout Create(
            const vk::ArrayProxy<vk::DescriptorSetLayout> &setLayouts,
            const vk::ArrayProxy<vk::PushConstantRange>   &pushConstants,
            const vk::Device                              &device);
        vk::PipelineLayout Create(
            const vk::ArrayProxy<vk::DescriptorSetLayout> &setLayouts,
            const vk::Device                              &device);
    }  // namespace PipelineLayout

    namespace ComputePipeline {
        vk::Pipeline Create(
            const std::string        &entry,
            const vk::ShaderModule   &module,
            const vk::PipelineLayout &layout,
            const vk::Device         &device);
    }  // namespace ComputePipeline

    namespace GraphicsPipeline {
        class Builder {
           public:
            Builder();
            ~Builder() = default;

            void clear();

            Builder &setShaders(const std::vector<Shader::StageInfo> &shaders);
            Builder &setVertexShader(const std::string &entry, const vk::ShaderModule &shader);
            Builder &setFragmentShader(const std::string &entry, const vk::ShaderModule &shader);
            Builder &setVertexLayouts(const std::vector<VertexLayout> &layouts);
            Builder &setVertexLayout(const VertexLayout &layout);
            Builder &setInputTopology(vk::PrimitiveTopology topology);
            Builder &setPolygonMode(vk::PolygonMode mode);
            Builder &setCullMode(vk::CullModeFlags cullMode, vk::FrontFace frontFace);
            Builder &setColorAttachmentFormats(const std::vector<vk::Format> &formats);
            Builder &setColorAttachmentFormat(vk::Format format);
            Builder &setDepthFormat(vk::Format format);
            Builder &setDepthTest(bool depthWrite, vk::CompareOp op);
            Builder &setBlendingAdditive();
            Builder &setBlendingAlphaBlended();
            Builder &setNoMultisampling();
            Builder &setNoBlending();
            Builder &setNoDepthTest();

            vk::Pipeline build(const vk::PipelineLayout &layout, const vk::Device &device);

           private:
            std::vector<Shader::StageInfo>                   m_ShaderStages;
            std::vector<vk::VertexInputBindingDescription>   m_VertexInputBindingDescriptions;
            std::vector<vk::VertexInputAttributeDescription> m_VertexInputAttributeDescriptions;
            vk::PipelineInputAssemblyStateCreateInfo         m_InputAssembly;
            vk::PipelineRasterizationStateCreateInfo         m_Rasterizer;
            vk::PipelineColorBlendAttachmentState            m_ColorBlendAttachment;
            vk::PipelineMultisampleStateCreateInfo           m_Multisampling;
            vk::PipelineDepthStencilStateCreateInfo          m_DepthStencil;
            vk::PipelineRenderingCreateInfo                  m_RenderingInfo;
            std::vector<vk::Format>                          m_ColorAttachmentFormats;
        };
    }  // namespace GraphicsPipeline
}  // namespace vre::Vulkan