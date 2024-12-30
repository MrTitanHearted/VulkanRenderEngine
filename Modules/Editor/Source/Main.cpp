#include <VREngine/Editor.hpp>

int main(int argc, char **argv) {
    vre::Logger::Initialize();
    vre::AssetServer::Initialize();
    vre::EventObserver::Initialize();
    vre::Window::Initialize({
        .Title = "Vulkan Render Engine Editor",
    });
    vre::Vulkan::Context::Initialize({});

    vre::Editor editor{};

    try {
        editor.init();
        editor.run();
        editor.release();
    } catch (const std::exception &e) {
        LOG_FATAL("Exception: '{}'", e.what());
    } catch (...) {
        LOG_FATAL("Unkown Exception");
    }

    vre::Vulkan::Context::Shutdown();
    vre::Window::Shutdown();
    vre::EventObserver::Shutdown();
    vre::AssetServer::Shutdown();
    vre::Logger::Shutdown();
    return 0;
}

#if defined(VRE_PLATFORM_WINDOWS) && defined(VRE_BUILD_TYPE_RELEASE)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow) {
    return main(__argc, __argv);
}
#endif