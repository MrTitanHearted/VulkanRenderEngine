#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdarg>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <source_location>
#include <vector>
#include <array>
#include <span>
#include <string>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <tuple>
#include <utility>
#include <chrono>
#include <memory>
#include <format>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <limits>
#include <thread>
#include <cmath>

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/types.hpp>
#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/util.hpp>

namespace fs = std::filesystem;

#if defined(_WIN32) || defined(_WIN64)
#define VRE_PLATFORM_WINDOWS
#elif defined(__linux__)
#define VRE_PLATFORM_LINUX
#define VRE_PLATFORM_UNIX
#else
#error Unsupported platform!
#endif

#if !defined(VRE_BUILD_TYPE_RELEASE) && !defined(VRE_BUILD_TYPE_DEBUG)
#define VRE_BUILD_TYPE_DEBUG
#endif

#define VRE_ENUM_CLASS_BITWISE_OPERATORS(Type, Other)                            \
    inline Type operator|(Type a, Type b) {                                      \
        return static_cast<Type>(static_cast<Other>(a) | static_cast<Other>(b)); \
    }                                                                            \
    inline Type operator|=(Type &a, Type b) {                                    \
        return a = a | b;                                                        \
    }                                                                            \
    inline Type operator&(Type a, Type b) {                                      \
        return static_cast<Type>(static_cast<Other>(a) & static_cast<Other>(b)); \
    }                                                                            \
    inline Type operator&=(Type &a, Type b) {                                    \
        return a = a & b;                                                        \
    }                                                                            \
    inline Type operator^(Type a, Type b) {                                      \
        return static_cast<Type>(static_cast<Other>(a) ^ static_cast<Other>(b)); \
    }                                                                            \
    inline Type operator^=(Type &a, Type b) {                                    \
        return a = a ^ b;                                                        \
    }                                                                            \
    inline Type operator~(Type a) {                                              \
        return static_cast<Type>(~static_cast<Other>(a));                        \
    }

#define VRE_ENUM_UINT_BITWISE_OPERATORS(Type) VRE_ENUM_CLASS_BITWISE_OPERATORS(Type, std::uint32_t)

#ifdef VRE_BUILD_TYPE_DEBUG
#ifndef VRE_IF_DEBUG
#define VRE_IF_DEBUG(exp) exp
#endif
#else
#ifndef VRE_IF_DEBUG
#define VRE_IF_DEBUG(exp)
#endif
#endif