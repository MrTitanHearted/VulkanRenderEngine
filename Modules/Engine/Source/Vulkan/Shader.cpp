#include <VREngine/Vulkan/Shader.hpp>

namespace vre::Vulkan::Shader {
    vk::ShaderModule CreateSPV(const std::string &source, const vk::Device &device) {
        auto [result, module] = device.createShaderModule(vk::ShaderModuleCreateInfo{
            {},
            source.size(),
            (std::uint32_t *)source.data(),
        });
        DVRE_VK_CHECK(result);
        return module;
    }
}  // namespace vre::Vulkan::Shader