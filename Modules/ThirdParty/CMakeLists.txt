cmake_minimum_required(VERSION 3.20)

project(VulkanRenderEngineThirdParty)

add_subdirectory(VulkanMemoryAllocator)

find_package(fastgltf CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(Stb REQUIRED)

set(VULKAN_RENDER_ENGINE_THIRD_PARTY_IMGUI_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imconfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_internal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_rectpack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_textedit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.h

    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_vulkan.h
)
set(VULKAN_RENDER_ENGINE_THIRD_PARTY_IMGUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_vulkan.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/imgui PREFIX Include FILES ${VULKAN_RENDER_ENGINE_THIRD_PARTY_IMGUI_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/imgui PREFIX Source FILES ${VULKAN_RENDER_ENGINE_THIRD_PARTY_IMGUI_SOURCES})

add_library(VulkanRenderEngineThirdParty STATIC ${VULKAN_RENDER_ENGINE_THIRD_PARTY_IMGUI_HEADERS} ${VULKAN_RENDER_ENGINE_THIRD_PARTY_IMGUI_SOURCES})
add_library(VulkanRenderEngine::ThirdParty ALIAS VulkanRenderEngineThirdParty)

target_link_libraries(VulkanRenderEngineThirdParty PUBLIC
    glfw
    Vulkan::Vulkan
    GPUOpen::VulkanMemoryAllocator
    glm::glm
    fastgltf::fastgltf
)
target_include_directories(VulkanRenderEngineThirdParty PUBLIC
    ${Vulkan_INCLUDE_DIRS}
    ${Stb_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

if(WIN32)
    target_link_libraries(VulkanRenderEngineThirdParty PUBLIC Dwmapi)
endif()