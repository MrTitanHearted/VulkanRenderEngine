#pragma once

#include <VREngine/Core.hpp>
#include <VREngine/Assets/AssetHandle.hpp>

namespace vre {
    class TextureAsset : public IAsset {
       public:
        static TextureAsset FromData(const fs::path &path, void *data, std::size_t size, bool flipVertically = true);
        static TextureAsset FromPath(const fs::path &path, bool flipVertically = true);

       public:
        TextureAsset(const fs::path &path, void *data, std::size_t size, std::uint32_t width, std::uint32_t height, std::uint32_t channelCount, std::uint32_t stride);

        TextureAsset()  = default;
        ~TextureAsset() = default;

        void release() override;

        std::string getPath() const;
        std::string getDirectory() const;
        std::string getName() const;
        std::string getExtension() const;

        void         *getData() const;
        std::size_t   getSize() const;
        std::uint32_t getWidth() const;
        std::uint32_t getHeight() const;
        std::uint32_t getChannelCount() const;
        std::uint32_t getStride() const;

       private:
        std::string m_Path;
        std::string m_Directory;
        std::string m_Name;
        std::string m_Extension;

        void         *m_Data;
        std::size_t   m_Size;
        std::uint32_t m_Width;
        std::uint32_t m_Height;
        std::uint32_t m_ChannelCount;
        std::uint32_t m_Stride;
    };
}  // namespace vre