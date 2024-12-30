#include <VREngine/Window.hpp>

#if defined(VRE_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <dwmapi.h>
#endif

namespace vre {
    GLFWwindow   *Window::g_Window{nullptr};
    std::uint32_t Window::g_MinWidth{0};
    std::uint32_t Window::g_MinHeight{0};
    std::uint32_t Window::g_MaxWidth{0xFFFFFFFF};
    std::uint32_t Window::g_MaxHeight{0xFFFFFFFF};
    std::uint32_t Window::g_PrevWidth{0};
    std::uint32_t Window::g_PrevHeight{0};
    std::uint32_t Window::g_PrevX{0};
    std::uint32_t Window::g_PrevY{0};
    KeyMod        Window::g_KeyMods{KeyMod::eNone};

    std::array<bool, GLFW_KEY_LAST + 1>          Window::g_Keys{};
    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> Window::g_MouseButtons{};

    bool   Window::g_IsInitialized{false};
    Window Window::g_State{};

    void Window::Initialize(const Settings &settings) {
        VRE_ASSERT(glfwInit() == GLFW_TRUE, "Failed to initialize Glfw3");
        DVRE_ASSERT(EventObserver::IsInitialized(), "vre::EventObserver must be initialized");
        DVRE_ASSERT(!g_IsInitialized, "vre::Window can be initialized once only after shutting down or for the first time");
        DLOG_INFO("Initializing vre::Window with Title: '{}', Resolution: '{}x{}', Fullscreen: {}", settings.Title, settings.Width, settings.Height, settings.Fullscreen);

        glfwSetErrorCallback(GlfwErrorCallback);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, settings.Resizable ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, settings.Transparent ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_VISIBLE, settings.Visible ? GLFW_TRUE : GLFW_FALSE);
        std::uint32_t width   = settings.Width;
        std::uint32_t height  = settings.Height;
        GLFWmonitor  *monitor = settings.Fullscreen ? glfwGetPrimaryMonitor() : nullptr;
        if (settings.Fullscreen && settings.FullscreenMonitorSize && monitor != nullptr) {
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            width                   = mode->width;
            height                  = mode->height;
        }

        g_Window = glfwCreateWindow(width, height, settings.Title.c_str(), monitor, nullptr);
        if (settings.Visible)
            glfwShowWindow(g_Window);

        glfwSetWindowCloseCallback(g_Window, GlfwWindowCloseCallback);
        glfwSetWindowSizeCallback(g_Window, GlfwWindowSizeCallback);
        glfwSetWindowPosCallback(g_Window, GlfwWindowPositionCallback);
        glfwSetWindowContentScaleCallback(g_Window, GlfwWindowContentScaleCallback);
        glfwSetWindowPosCallback(g_Window, GlfwWindowPositionCallback);
        glfwSetWindowMaximizeCallback(g_Window, GlfwWindowMaximizeCallback);
        glfwSetWindowIconifyCallback(g_Window, GlfwWindowMinimizeCallback);
        glfwSetWindowFocusCallback(g_Window, GlfwWindowFocusCallback);
        glfwSetWindowRefreshCallback(g_Window, GlfwWindowRefreshCallback);
        glfwSetKeyCallback(g_Window, GlfwWindowKeyCallback);
        glfwSetCharCallback(g_Window, GlfwWindowCharCallback);
        glfwSetCursorPosCallback(g_Window, GlfwWindowMousePositionCallback);
        glfwSetCursorEnterCallback(g_Window, GlfwWindowMouseEnterCallback);
        glfwSetMouseButtonCallback(g_Window, GlfwWindowMouseButtonCallback);
        glfwSetScrollCallback(g_Window, GlfwWindowMouseScrollCallback);
        glfwSetDropCallback(g_Window, GlfwWindowDropCallback);

#ifdef VRE_PLATFORM_WINDOWS
        {
            HWND hWnd    = glfwGetWin32Window(g_Window);
            BOOL enabled = TRUE;
            DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &enabled, sizeof(BOOL));
        }
#endif

        g_KeyMods = KeyMod::eNone;
        std::fill(std::begin(g_Keys), std::end(g_Keys), false);
        std::fill(std::begin(g_MouseButtons), std::end(g_MouseButtons), false);

        g_MinWidth   = 0;
        g_MinHeight  = 0;
        g_MaxWidth   = 0xFFFFFFFF;
        g_MaxHeight  = 0xFFFFFFFF;
        g_PrevWidth  = 0;
        g_PrevHeight = 0;
        g_PrevX      = 0;
        g_PrevY      = 0;

        if (monitor != nullptr) {
            g_PrevWidth  = width / 2;
            g_PrevHeight = height / 2;
            g_PrevX      = (width - width) / 2;
            g_PrevY      = (height - height) / 2;
        }

        g_IsInitialized = true;
    }

    void Window::Shutdown() {
        VRE_ASSERT(g_IsInitialized, "vre::Window can be shut down once only after initializing");
        DLOG_INFO("Shutting vre::Window with Title: '{}' down", GetTitle());

        glfwDestroyWindow(g_Window);
        glfwTerminate();

        g_IsInitialized = false;
    }

    bool Window::IsInitialized() {
        return g_IsInitialized;
    }

    void Window::PollEvents() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window::PollEvents() can only be used after initializing vre::Window");
        glfwPollEvents();
    }

    void Window::SetTitle(const std::string &title) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwSetWindowTitle(g_Window, title.c_str());
    }

    void Window::SetClipboardString(const std::string &str) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwSetClipboardString(g_Window, str.c_str());
    }

    void Window::SetSize(std::uint32_t width, std::uint32_t height) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwSetWindowSize(g_Window, width, height);
    }

    void Window::SetWidth(std::uint32_t width) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        std::int32_t height = 0;
        glfwGetWindowSize(g_Window, nullptr, &height);
        glfwSetWindowSize(g_Window, width, height);
    }

    void Window::SetHeight(std::uint32_t height) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        std::int32_t width = 0;
        glfwGetWindowSize(g_Window, &width, nullptr);
        glfwSetWindowSize(g_Window, width, height);
    }

    void Window::SetSizeLimits(std::uint32_t minWidth, std::uint32_t minHeight, std::uint32_t maxWidth, std::uint32_t maxHeight) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        DVRE_ASSERT(minWidth < maxWidth, "Minimum Width must be less than Maximum Width");
        DVRE_ASSERT(minHeight < maxHeight, "Minimum Height must be less than Maximum Height");

        g_MinWidth  = minWidth;
        g_MinHeight = minHeight;
        g_MaxWidth  = maxWidth;
        g_MaxHeight = maxHeight;

        glfwSetWindowSizeLimits(g_Window, g_MinWidth, g_MinHeight, g_MaxWidth, g_MaxHeight);
    }

    void Window::SetMinSizeLimit(std::uint32_t minWidth, std::uint32_t minHeight) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        DVRE_ASSERT(minWidth < g_MaxWidth, "Minimum Width must be less than Maximum Width");
        DVRE_ASSERT(minHeight < g_MaxHeight, "Minimum Height must be less than Maximum Height");

        g_MinWidth  = minWidth;
        g_MinHeight = minHeight;

        glfwSetWindowSizeLimits(g_Window, g_MinWidth, g_MinHeight, g_MaxWidth, g_MaxHeight);
    }

    void Window::SetMazSizeLimit(std::uint32_t maxWidth, std::uint32_t maxHeight) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        DVRE_ASSERT(g_MinWidth < maxWidth, "Minimum Width must be less than Maximum Width");
        DVRE_ASSERT(g_MinHeight < maxHeight, "Minimum Height must be less than Maximum Height");

        g_MaxWidth  = maxWidth;
        g_MaxHeight = maxHeight;

        glfwSetWindowSizeLimits(g_Window, g_MinWidth, g_MinHeight, g_MaxWidth, g_MaxHeight);
    }

    void Window::SetPosition(std::uint32_t x, std::uint32_t y) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwSetWindowPos(g_Window, x, y);
    }

    void Window::SetX(std::uint32_t x) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        std::int32_t y = 0;
        glfwGetWindowPos(g_Window, nullptr, &y);
        glfwSetWindowPos(g_Window, x, y);
    }

    void Window::SetY(std::uint32_t y) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        std::int32_t x = 0;
        glfwGetWindowPos(g_Window, &x, nullptr);
        glfwSetWindowPos(g_Window, x, y);
    }

    void Window::SetMousePosition(double x, double y) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwSetCursorPos(g_Window, x, y);
    }

    void Window::SetMouseX(double x) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        double y = 0.0;
        glfwGetCursorPos(g_Window, nullptr, &y);
        glfwSetCursorPos(g_Window, x, y);
    }

    void Window::SetMouseY(double y) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        double x = 0.0;
        glfwGetCursorPos(g_Window, &x, nullptr);
        glfwSetCursorPos(g_Window, x, y);
    }

    void Window::SetCursorMode(CursorMode mode) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwSetInputMode(g_Window, GLFW_CURSOR, static_cast<std::int32_t>(mode));
    }

    void Window::SetTransparency(float transparency) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        DVRE_ASSERT(0.0f <= transparency && transparency <= 1.0f, "vre::Window can be transparent between ranges of 0.0f and 1.0f");
        glfwSetWindowOpacity(g_Window, 1.0f - transparency);
    }

    void Window::SetOpacity(float opacity) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        DVRE_ASSERT(0.0f <= opacity && opacity <= 1.0f, "vre::Window can be opaque between ranges of 0.0f and 1.0f");
        glfwSetWindowOpacity(g_Window, opacity);
    }

    void Window::SetVisible(bool visible) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwSetWindowAttrib(g_Window, GLFW_VISIBLE, visible ? GLFW_TRUE : GLFW_FALSE);
    }

    void Window::SetResizable(bool resizable) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwSetWindowAttrib(g_Window, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
    }

    void Window::SetRunning(bool running) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwSetWindowShouldClose(g_Window, running ? GLFW_FALSE : GLFW_TRUE);
    }

    void Window::SetLockKeyMods(bool lock) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwSetInputMode(g_Window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
    }

    void Window::MakeFullscreen(bool monitorSize) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");

        std::int32_t width  = 0;
        std::int32_t height = 0;
        std::int32_t x      = 0;
        std::int32_t y      = 0;

        glfwGetWindowSize(g_Window, &width, &height);
        glfwGetWindowPos(g_Window, &x, &y);

        g_PrevWidth  = width;
        g_PrevHeight = height;
        g_PrevX      = x;
        g_PrevY      = y;

        GLFWmonitor       *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode    = glfwGetVideoMode(monitor);
        if (monitorSize) {
            width  = mode->width;
            height = mode->height;
        }

        glfwSetWindowMonitor(g_Window, monitor, 0, 0, width, height, mode->refreshRate);
    }

    void Window::MakeWindowed() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwSetWindowMonitor(g_Window, nullptr, g_PrevX, g_PrevY, g_PrevWidth, g_PrevHeight, 0);
    }

    void Window::Maximize() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwMaximizeWindow(g_Window);
    }

    void Window::Minimize() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwIconifyWindow(g_Window);
    }

    void Window::Restore() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwRestoreWindow(g_Window);
    }

    void Window::Close() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwSetWindowShouldClose(g_Window, GLFW_TRUE);
    }

    void Window::RequestWindowAttention() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwRequestWindowAttention(g_Window);
    }

    void Window::ShowWindow() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwShowWindow(g_Window);
    }

    void Window::HideWindow() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        glfwHideWindow(g_Window);
    }

    GLFWwindow *Window::GetGlfwWindow() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return g_Window;
    }

    std::string Window::GetTitle() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return glfwGetWindowTitle(g_Window);
    }

    std::string Window::GetClipboardString() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return glfwGetClipboardString(nullptr);
    }

    std::array<std::uint32_t, 2> Window::GetSize() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        std::int32_t width  = 0;
        std::int32_t height = 0;
        glfwGetWindowSize(g_Window, &width, &height);
        return {static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height)};
    }

    std::uint32_t Window::GetWidth() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        std::int32_t width = 0;
        glfwGetWindowSize(g_Window, &width, nullptr);
        return width;
    }

    std::uint32_t Window::GetHeight() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        std::int32_t height = 0;
        glfwGetWindowSize(g_Window, nullptr, &height);
        return height;
    }

    std::array<std::uint32_t, 2> Window::GetMinSizeLimit() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return {g_MinWidth, g_MinHeight};
    }

    std::array<std::uint32_t, 2> Window::GetMaxSizeLimit() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return {g_MaxWidth, g_MaxHeight};
    }

    std::array<std::uint32_t, 2> Window::GetPosition() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        std::int32_t x = 0;
        std::int32_t y = 0;
        glfwGetWindowPos(g_Window, &x, &y);
        return {static_cast<std::uint32_t>(x), static_cast<std::uint32_t>(y)};
    }

    std::uint32_t Window::GetX() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        std::int32_t x = 0;
        glfwGetWindowPos(g_Window, &x, nullptr);
        return x;
    }

    std::uint32_t Window::GetY() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        std::int32_t y = 0;
        glfwGetWindowPos(g_Window, nullptr, &y);
        return y;
    }

    std::array<double, 2> Window::GetMousePosition() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        double x = 0.0;
        double y = 0.0;
        glfwGetCursorPos(g_Window, &x, &y);
        return {x, y};
    }

    double Window::GetMouseX() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        double x = 0.0;
        glfwGetCursorPos(g_Window, &x, nullptr);
        return x;
    }

    double Window::GetMouseY() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        double y = 0.0;
        glfwGetCursorPos(g_Window, nullptr, &y);
        return y;
    }

    CursorMode Window::GetCursorMode() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return static_cast<CursorMode>(glfwGetInputMode(g_Window, GLFW_CURSOR));
    }

    float Window::GetOpacity() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return glfwGetWindowOpacity(g_Window);
    }

    bool Window::IsClosed() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return glfwWindowShouldClose(g_Window) == GLFW_TRUE;
    }

    bool Window::IsRunning() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return glfwWindowShouldClose(g_Window) == GLFW_FALSE;
    }

    bool Window::IsMaximized() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return glfwGetWindowAttrib(g_Window, GLFW_MAXIMIZED) == GLFW_TRUE;
    }

    bool Window::IsMinimized() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return glfwGetWindowAttrib(g_Window, GLFW_ICONIFIED) == GLFW_TRUE;
    }

    bool Window::IsFocused() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return glfwGetWindowAttrib(g_Window, GLFW_FOCUSED) == GLFW_TRUE;
    }

    bool Window::IsResizable() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return glfwGetWindowAttrib(g_Window, GLFW_RESIZABLE) == GLFW_TRUE;
    }

    bool Window::IsTransparent() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return glfwGetWindowAttrib(g_Window, GLFW_TRANSPARENT_FRAMEBUFFER) == GLFW_TRUE;
    }

    bool Window::IsHovered() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return glfwGetWindowAttrib(g_Window, GLFW_HOVERED) == GLFW_TRUE;
    }

    bool Window::IsVisible() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return glfwGetWindowAttrib(g_Window, GLFW_VISIBLE) == GLFW_TRUE;
    }

    bool Window::IsFullscreen() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return glfwGetWindowMonitor(g_Window) != nullptr;
    }

    bool Window::IsKeyModLocked() {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return glfwGetInputMode(g_Window, GLFW_LOCK_KEY_MODS) == GLFW_TRUE;
    }

    bool Window::IsKeyModPressed(KeyMod mod) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return static_cast<std::int32_t>(g_KeyMods & mod) > 0;
    }

    bool Window::IsKeyDown(KeyCode key) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return g_Keys[static_cast<std::size_t>(key)];
    }

    bool Window::IsKeyUp(KeyCode key) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return !g_Keys[static_cast<std::size_t>(key)];
    }

    bool Window::IsMouseButtonDown(MouseButton button) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return g_MouseButtons[static_cast<std::size_t>(button)];
    }

    bool Window::IsMouseButtonUp(MouseButton button) {
        DVRE_ASSERT(g_IsInitialized, "vre::Window must be initialized");
        return !g_MouseButtons[static_cast<std::size_t>(button)];
    }

    Window::~Window() {
        VRE_ASSERT(!g_IsInitialized, "vre::Window must be shut down before closing!");
    }
}  // namespace vre