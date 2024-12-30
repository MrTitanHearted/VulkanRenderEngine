#pragma once

#include <VREngine/Core.hpp>
#include <VREngine/Assets/AssetHandle.hpp>

namespace vre {

    class AssetServer {
       public:
        static void Initialize();
        static void Shutdown();

        static bool IsInitialized();

        template <typename Asset, typename = std::enable_if_t<std::is_base_of<IAsset, Asset>::value>>
        static AssetHandle<Asset> Add(const Asset &asset) {
            DVRE_ASSERT(g_IsInitialized, "vre::AssetServer must be initialized");
            AssetVector<Asset> &assets = getAssetVector<Asset>();
            std::size_t         index  = assets.add(asset);
            return AssetHandle{asset, index};
        }

        template <typename Asset, typename = std::enable_if_t<std::is_base_of<IAsset, Asset>::value>>
        static bool Set(const AssetHandle<Asset> &asset) {
            DVRE_ASSERT(g_IsInitialized, "vre::AssetServer must be initialized");
            AssetVector<Asset> &assets = getAssetVector<Asset>();
            if (!assets.contains(asset.m_Index)) {
                DVRE_WARN("vre::AssetHandle<{}> with index {} is not added to vre::AssetServer", typeid(Asset).name(), asset.m_Index);
                return false;
            }
            assets[asset.m_Index] = asset.m_Data;
            return true;
        }

        template <typename Asset, typename = std::enable_if_t<std::is_base_of<IAsset, Asset>::value>>
        static bool Set(std::size_t index, const Asset &asset) {
            DVRE_ASSERT(g_IsInitialized, "vre::AssetServer must be initialized");
            AssetVector<Asset> &assets = getAssetVector<Asset>();
            if (!assets.contains(index)) {
                DVRE_WARN("vre::AssetHandle<{}> with index {} is not added to vre::AssetServer", typeid(Asset).name(), index);
                return false;
            }
            assets[index] = asset;
            return true;
        }

        template <typename Asset, typename = std::enable_if_t<std::is_base_of<IAsset, Asset>::value>>
        static bool Release(AssetHandle<Asset> &asset) {
            DVRE_ASSERT(g_IsInitialized, "vre::AssetServer must be initialized");
            std::size_t index          = asset.m_Index;
            asset.m_Index              = 0;
            AssetVector<Asset> &assets = getAssetVector<Asset>();
            if (!assets.contains(index)) {
                return false;
            }
            assets.remove(index);
            return true;
        }

        template <typename Asset, typename = std::enable_if_t<std::is_base_of<IAsset, Asset>::value>>
        static bool Release(std::size_t index) {
            DVRE_ASSERT(g_IsInitialized, "vre::AssetServer must be initialized");
            AssetVector<Asset> &assets = getAssetVector<Asset>();
            if (!assets.contains(index)) {
                return false;
            }
            assets.remove(index);
            return true;
        }

        template <typename Asset, typename = std::enable_if_t<std::is_base_of<IAsset, Asset>::value>>
        static bool Contains(AssetHandle<Asset> &asset) {
            DVRE_ASSERT(g_IsInitialized, "vre::AssetServer must be initialized");
            AssetVector<Asset> &assets = getAssetVector<Asset>();
            if (!assets.contains(asset.m_Index)) {
                asset.m_Index = 0;
                return false;
            }
            return true;
        }

        template <typename Asset, typename = std::enable_if_t<std::is_base_of<IAsset, Asset>::value>>
        static bool Contains(std::size_t index) {
            DVRE_ASSERT(g_IsInitialized, "vre::AssetServer must be initialized");
            AssetVector<Asset> &assets = getAssetVector<Asset>();
            return assets.contains(index);
        }

       private:
        class Status {
           public:
            Status() = default;
            ~Status();
        };

        class IAssetVector {
           public:
            virtual ~IAssetVector() = default;
            virtual void clear()    = 0;
        };

        template <typename Asset, typename = std::enable_if_t<std::is_base_of<IAsset, Asset>::value>>
        class AssetVector : public IAssetVector {
           public:
            AssetVector()  = default;
            ~AssetVector() = default;

            void add(std::size_t index, Asset &&data) {
                if (m_LookUp.find(index) != m_LookUp.end()) {
                    DVRE_WARN("vre::AssetHandle<{}> already exists with Index: {}", typeid(Asset).name(), index);
                    return;
                }

                m_LookUp[index] = m_Data.size();
                m_Data.emplace_back(std::forward<Asset>(data));
                m_Indices.emplace_back(index);
            }

            void set(std::size_t index, Asset &&data) {
                auto it = m_LookUp.find(index);
                if (it != m_LookUp.end()) {
                    m_Data[it->second] = std::forward<Asset>(data);
                    return;
                }

                m_LookUp[index] = m_Data.size();
                m_Data.emplace_back(std::forward<Asset>(data));
                m_Indices.emplace_back(index);
            }

            void remove(std::size_t index) {
                auto it = m_LookUp.find(index);
                if (it == m_LookUp.end()) return;

                const std::size_t i     = it->second;
                const std::size_t lastI = m_Data.size() - 1;
                if (i != lastI) {
                    m_Data[i]              = std::move(m_Data.back());
                    m_Indices[i]           = std::move(m_Indices.back());
                    m_LookUp[m_Indices[i]] = i;
                }

                m_Data[m_Data.size() - 1].release();
                m_LookUp.erase(index);
                m_Data.pop_back();
                m_Indices.pop_back();
            }

            virtual void clear() override {
                for (Asset &asset : m_Data) {
                    asset.release();
                }
                m_Data.clear();
                m_LookUp.clear();
                m_Indices.clear();
            }

            bool contains(std::size_t index) const {
                return m_LookUp.find(index) != m_LookUp.end();
            }

            Asset &operator[](std::size_t index) {
                auto it = m_LookUp.find(index);
                if (it == m_LookUp.end()) {
                    throw std::out_of_range("Index not found in SparseVector");
                }
                return m_Data[it->second];
            }

            const Asset &operator[](std::size_t index) const {
                auto it = m_LookUp.find(index);
                if (it == m_LookUp.end()) {
                    throw std::out_of_range("Index not found in SparseVector");
                }
                return m_Data[it->second];
            }

            std::size_t size() const {
                return m_Data.size();
            }

           private:
            std::vector<Asset>                 m_Data;
            std::vector<std::size_t>           m_Indices;
            std::map<std::size_t, std::size_t> m_LookUp;
        };

       private:
        static std::map<std::size_t, std::unique_ptr<IAssetVector>> g_AssetsMap;
        static bool                                                 g_IsInitialized;
        static Status                                               g_Status;

       private:
        AssetServer()  = default;
        ~AssetServer() = default;

        template <typename Asset, typename = std::enable_if_t<std::is_base_of<IAsset, Asset>::value>>
        static std::map<std::size_t, Asset> &getAssetVector() {
            std::size_t assetType = typeid(Asset).hash_code();

            if (g_AssetsMap.find(assetType) == g_AssetsMap.end()) {
                g_AssetsMap[assetType] = std::make_unique<AssetVector<Asset>>();
            }
            return *dynamic_cast<AssetVector<Asset> *>(g_AssetsMap[assetType].get());
        }
    };

    template <typename Asset, typename EnableIf>
    bool AssetHandle<Asset, EnableIf>::isAdded() const {
        return AssetServer::Contains<Asset>(m_Index);
    }
}  // namespace vre