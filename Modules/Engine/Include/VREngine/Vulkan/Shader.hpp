#pragma once

#include <VREngine/Vulkan/Types.hpp>

namespace vre::Vulkan::Shader {
    vk::ShaderModule CreateSPV(const std::string &source, const vk::Device &device);
}  // namespace vre::Vulkan::Shader