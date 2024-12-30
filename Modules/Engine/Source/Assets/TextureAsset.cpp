#include <VREngine/Assets/TextureAsset.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace vre {
    TextureAsset::TextureAsset(
        const fs::path &path, void *data, std::size_t size,
        std::uint32_t width, std::uint32_t height,
        std::uint32_t channelCount, std::uint32_t stride)
        : m_Path{path.string()}, m_Data{data}, m_Size{size}, m_Width{width}, m_Height{height}, m_ChannelCount{channelCount}, m_Stride{stride} {
        DVRE_INFO("Initializing a vre::TextureAsset from path: '{}'", m_Path);
        m_Directory = path.parent_path().string();
        m_Name      = path.filename().string();
        m_Extension = path.extension().string();
    }

    void TextureAsset::release() {
        DVRE_INFO("Releasing a vre::TextureAsset from path: '{}'", m_Path);
        stbi_image_free(m_Data);
    }

    std::string TextureAsset::getPath() const { return m_Path; }

    std::string TextureAsset::getDirectory() const { return m_Directory; }

    std::string TextureAsset::getName() const { return m_Name; }

    std::string TextureAsset::getExtension() const { return m_Extension; }

    void *TextureAsset::getData() const { return m_Data; }

    std::size_t TextureAsset::getSize() const { return m_Size; }

    std::uint32_t TextureAsset::getWidth() const { return m_Width; }

    std::uint32_t TextureAsset::getHeight() const { return m_Height; }

    std::uint32_t TextureAsset::getChannelCount() const { return m_ChannelCount; }

    std::uint32_t TextureAsset::getStride() const { return m_Stride; }

    TextureAsset TextureAsset::FromData(const fs::path &path, void *data, std::size_t size, bool flipVertically) {
        if (flipVertically) stbi_set_flip_vertically_on_load(1);

        std::int32_t width   = 0;
        std::int32_t height  = 0;
        void        *rawData = stbi_load_from_memory((const stbi_uc *)data, size, &width, &height, nullptr, STBI_rgb_alpha);

        VRE_ASSERT(rawData != nullptr, "Failed to load a vre::TextureAsset file from memory");

        if (flipVertically) stbi_set_flip_vertically_on_load(0);

        return TextureAsset{path, rawData, std::uint32_t(width * height * 4), std::uint32_t(width), std::uint32_t(height), 4u, std::uint32_t(width) * 4u};
    }

    TextureAsset TextureAsset::FromPath(const fs::path &path, bool flipVertically) {
        VRE_ASSERT(fs::exists(path), "Failed to find a texture from path: '{}'", path.string());

        if (flipVertically) stbi_set_flip_vertically_on_load(1);

        std::int32_t width   = 0;
        std::int32_t height  = 0;
        void        *rawData = stbi_load(path.string().c_str(), &width, &height, nullptr, STBI_rgb_alpha);

        VRE_ASSERT(rawData != nullptr, "Failed to load a vre::TextureAsset file from path: '{}'", path.string());

        if (flipVertically) stbi_set_flip_vertically_on_load(0);

        return TextureAsset{path, rawData, std::uint32_t(width * height * 4), std::uint32_t(width), std::uint32_t(height), 4u, std::uint32_t(width) * 4u};
    }

}  // namespace vre