#pragma once

#include <VREngine/Core.hpp>
#include <VREngine/Assets/AssetHandle.hpp>

namespace vre {
    class FileAsset : public IAsset {
       public:
        static FileAsset FromContent(const fs::path &path, const std::string &content);
        static FileAsset FromPath(const fs::path &path);
        static FileAsset FromPathBinary(const fs::path &path);

       public:
        FileAsset(const fs::path &path, const std::string &content);

        FileAsset()  = default;
        ~FileAsset() = default;

        void release() override;

        std::string getPath() const;
        std::string getDirectory() const;
        std::string getName() const;
        std::string getExtension() const;
        std::string getContent() const;

        const std::string &operator*() const;
        operator const std::string &() const;

       private:
        std::string m_Path;
        std::string m_Directory;
        std::string m_Name;
        std::string m_Extension;
        std::string m_Content;
    };
}  // namespace vre