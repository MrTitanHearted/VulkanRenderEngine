#include <VREngine/Assets/AssetServer.hpp>

namespace vre {
    std::map<std::size_t, std::unique_ptr<AssetServer::IAssetVector>> AssetServer::g_AssetsMap{};
    bool                                                              AssetServer::g_IsInitialized{false};
    AssetServer::Status                                               AssetServer::g_Status{};

    void AssetServer::Initialize() {
        DVRE_ASSERT(!g_IsInitialized, "vre::AssetServer must be shut down before initializing");
        DLOG_INFO("Initializing vre::AssetServer");
        g_IsInitialized = true;
    }

    void AssetServer::Shutdown() {
        DVRE_ASSERT(g_IsInitialized, "vre::AssetServer must be initialized before shutting down");
        DLOG_INFO("Shutting vre::AssetServer down");

        for (auto &[assetType, assetVector] : g_AssetsMap) {
            assetVector->clear();
        }
        g_AssetsMap.clear();
        g_IsInitialized = false;
    }

    bool AssetServer::IsInitialized() {
        return g_IsInitialized;
    }

    AssetServer::Status::~Status() {
        VRE_ASSERT(!g_IsInitialized, "vre::AssetServer must be shut down before closing!");
    }
}  // namespace vre