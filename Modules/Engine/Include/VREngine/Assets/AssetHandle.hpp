#pragma once

#include <VREngine/Core.hpp>

namespace vre {
    class IAsset {
       public:
        virtual ~IAsset()      = default;
        virtual void release() = 0;
    };

    template <typename Asset, typename = std::enable_if_t<std::is_base_of<IAsset, Asset>::value>>
    class AssetHandle {
       public:
        AssetHandle()  = default;
        ~AssetHandle() = default;

        bool isAdded() const;

        std::size_t getIndex() const {
            DVRE_ASSERT(isAdded(), "vre::AssetHandle<{}> must be added to vre::AssetServer", typeid(Asset).name());
            return m_Index;
        }

        Asset &getRef() {
            DVRE_ASSERT(isAdded(), "vre::AssetHandle<{}> must be added to vre::AssetServer", typeid(Asset).name());
            return m_Data;
        }

        const Asset &getConstRef() {
            DVRE_ASSERT(isAdded(), "vre::AssetHandle<{}> must be added to vre::AssetServer", typeid(Asset).name());
            return m_Data;
        }

        operator Asset &() {
            DVRE_ASSERT(isAdded(), "vre::AssetHandle<{}> must be added to vre::AssetServer", typeid(Asset).name());
            return m_Data;
        }

        operator const Asset &() const {
            DVRE_ASSERT(isAdded(), "vre::AssetHandle<{}> must be added to vre::AssetServer", typeid(Asset).name());
            return m_Data;
        }

        Asset *operator->() {
            DVRE_ASSERT(isAdded(), "vre::AssetHandle<{}> must be added to vre::AssetServer", typeid(Asset).name());
            return &m_Data;
        }

        operator bool() const { return isAdded(); }

       private:
        Asset       m_Data;
        std::size_t m_Index;

       private:
        AssetHandle(const Asset &asset, std::size_t index)
            : m_Data{asset}, m_Index{index} {}

       private:
        friend class AssetServer;
    };
}  // namespace vre