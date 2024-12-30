#include <VREngine/Assets/FileAsset.hpp>

namespace vre {
    FileAsset::FileAsset(const fs::path &path, const std::string &content)
        : m_Path{path.string()}, m_Content{content} {
        DVRE_INFO("Initializing vre::FileAsset from path: '{}'", m_Path);
        m_Directory = path.parent_path().string();
        m_Name      = path.filename().string();
        m_Extension = path.extension().string();
    }

    void FileAsset::release() {
        DVRE_INFO("Releasing vre::FileAsset from path: '{}'", m_Path);
    }

    std::string FileAsset::getPath() const { return m_Path; }

    std::string FileAsset::getDirectory() const { return m_Directory; }

    std::string FileAsset::getName() const { return m_Name; }

    std::string FileAsset::getExtension() const { return m_Extension; }

    std::string FileAsset::getContent() const { return m_Content; }

    const std::string &FileAsset::operator*() const {
        return m_Content;
    }

    FileAsset::operator const std::string &() const {
        return m_Content;
    }

    FileAsset FileAsset::FromContent(const fs::path &path, const std::string &content) {
        return FileAsset{path, content};
    }

    FileAsset FileAsset::FromPath(const fs::path &path) {
        VRE_ASSERT(fs::exists(path), "Failed to find a file from path: '{}'", path.string());

        std::ifstream ifile{path, std::ios::ate};
        DVRE_ASSERT(ifile.is_open(), "Failed to open a file from path: '{}'", path.string());

        std::size_t   fileSize = ifile.tellg();
        std::string   content{};
        content.resize(fileSize);
        ifile.seekg(0);
        ifile.read(content.data(), fileSize);
        ifile.close();
        
        return FileAsset{path, content};
    }

    FileAsset FileAsset::FromPathBinary(const fs::path &path) {
        VRE_ASSERT(fs::exists(path), "Failed to find a file from path: '{}'", path.string());

        std::ifstream ifile{path, std::ios::ate | std::ios::binary};
        DVRE_ASSERT(ifile.is_open(), "Failed to open a file from path: '{}'", path.string());

        std::size_t fileSize = ifile.tellg();
        std::string content{};
        content.resize(fileSize);
        ifile.seekg(0);
        ifile.read(content.data(), fileSize);
        ifile.close();

        return FileAsset{path, content};
    }
}  // namespace vre