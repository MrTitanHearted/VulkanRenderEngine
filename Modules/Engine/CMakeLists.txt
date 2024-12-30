cmake_minimum_required(VERSION 3.20)

project(VulkanRenderEngine LANGUAGES CXX VERSION 0.0.1)

file(GLOB_RECURSE VULKAN_RENDER_ENGINE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Include/*.hpp)
file(GLOB_RECURSE VULKAN_RENDER_ENGINE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VULKAN_RENDER_ENGINE_HEADERS} ${VULKAN_RENDER_ENGINE_SOURCES})

if(BUILD_SHARED_LIBS)
    add_library(VulkanRenderEngine SHARED ${VULKAN_RENDER_ENGINE_HEADERS} ${VULKAN_RENDER_ENGINE_SOURCES})
else()
    add_library(VulkanRenderEngine STATIC ${VULKAN_RENDER_ENGINE_HEADERS} ${VULKAN_RENDER_ENGINE_SOURCES})
endif()

add_library(VulkanRenderEngine::VulkanRenderEngine ALIAS VulkanRenderEngine)

target_include_directories(VulkanRenderEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
target_link_libraries(VulkanRenderEngine PUBLIC VulkanRenderEngineThirdParty)