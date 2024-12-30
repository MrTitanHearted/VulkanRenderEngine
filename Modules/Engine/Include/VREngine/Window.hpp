#pragma once

#include <VREngine/Core.hpp>
#include <VREngine/Window/Enums.hpp>
#include <VREngine/Window/Events.hpp>

namespace vre {
    class Window {
       public:
        struct Settings {
            std::string   Title  = "Vulkan Render Engine";
            std::uint32_t Width  = 1000;
            std::uint32_t Height = 800;

            bool Resizable             = true;
            bool Fullscreen            = false;
            bool FullscreenMonitorSize = true;
            bool Transparent           = false;
            bool Visible               = true;
        };

       public:
        static void Initialize(const Settings &settings);
        static void Shutdown();

        static bool IsInitialized();

        static void PollEvents();

        static void SetTitle(const std::string &title);
        static void SetClipboardString(const std::string &str);
        static void SetSize(std::uint32_t width, std::uint32_t height);
        static void SetWidth(std::uint32_t width);
        static void SetHeight(std::uint32_t height);
        static void SetSizeLimits(std::uint32_t minWidth, std::uint32_t minHeight, std::uint32_t maxWidth, std::uint32_t maxHeight);
        static void SetMinSizeLimit(std::uint32_t minWidth, std::uint32_t minHeight);
        static void SetMazSizeLimit(std::uint32_t maxWidth, std::uint32_t maxHeight);
        static void SetPosition(std::uint32_t x, std::uint32_t y);
        static void SetX(std::uint32_t x);
        static void SetY(std::uint32_t y);
        static void SetMousePosition(double x, double y);
        static void SetMouseX(double x);
        static void SetMouseY(double y);
        static void SetCursorMode(CursorMode mode);
        static void SetTransparency(float transparency);
        static void SetOpacity(float opacity);
        static void SetVisible(bool visible);
        static void SetResizable(bool resizable);
        static void SetRunning(bool running);
        static void SetLockKeyMods(bool lock);
        static void MakeFullscreen(bool monitorSize = true);
        static void MakeWindowed();
        static void Maximize();
        static void Minimize();
        static void Restore();
        static void Close();
        static void RequestWindowAttention();
        static void ShowWindow();
        static void HideWindow();

        static GLFWwindow                  *GetGlfwWindow();
        static std::string                  GetTitle();
        static std::string                  GetClipboardString();
        static std::array<std::uint32_t, 2> GetSize();
        static std::uint32_t                GetWidth();
        static std::uint32_t                GetHeight();
        static std::array<std::uint32_t, 2> GetMinSizeLimit();
        static std::array<std::uint32_t, 2> GetMaxSizeLimit();
        static std::array<std::uint32_t, 2> GetPosition();
        static std::uint32_t                GetX();
        static std::uint32_t                GetY();
        static std::array<double, 2>        GetMousePosition();
        static double                       GetMouseX();
        static double                       GetMouseY();
        static CursorMode                   GetCursorMode();
        static float                        GetOpacity();

        static bool IsClosed();
        static bool IsRunning();
        static bool IsMaximized();
        static bool IsMinimized();
        static bool IsFocused();
        static bool IsResizable();
        static bool IsTransparent();
        static bool IsHovered();
        static bool IsVisible();
        static bool IsFullscreen();
        static bool IsKeyModLocked();

        static bool IsKeyModPressed(KeyMod mod);
        static bool IsKeyDown(KeyCode key);
        static bool IsKeyUp(KeyCode key);
        static bool IsMouseButtonDown(MouseButton button);
        static bool IsMouseButtonUp(MouseButton button);

       private:
        static GLFWwindow *g_Window;

        static std::uint32_t g_MinWidth;
        static std::uint32_t g_MinHeight;
        static std::uint32_t g_MaxWidth;
        static std::uint32_t g_MaxHeight;
        static std::uint32_t g_PrevWidth;
        static std::uint32_t g_PrevHeight;
        static std::uint32_t g_PrevX;
        static std::uint32_t g_PrevY;

        static KeyMod                                       g_KeyMods;
        static std::array<bool, GLFW_KEY_LAST + 1>          g_Keys;
        static std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> g_MouseButtons;

        static bool   g_IsInitialized;
        static Window g_State;

       private:
        Window() = default;
        ~Window();

        static void GlfwErrorCallback(std::int32_t errorCode, const char *description);

        static void GlfwWindowCloseCallback(GLFWwindow *window);
        static void GlfwWindowSizeCallback(GLFWwindow *window, std::int32_t width, std::int32_t height);
        static void GlfwWindowContentScaleCallback(GLFWwindow *window, float xScale, float yScale);
        static void GlfwWindowPositionCallback(GLFWwindow *window, std::int32_t x, std::int32_t y);
        static void GlfwWindowMaximizeCallback(GLFWwindow *window, std::int32_t isMaximized);
        static void GlfwWindowMinimizeCallback(GLFWwindow *window, std::int32_t isMinimized);
        static void GlfwWindowFocusCallback(GLFWwindow *window, std::int32_t focused);
        static void GlfwWindowRefreshCallback(GLFWwindow *window);
        static void GlfwWindowKeyCallback(GLFWwindow *window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods);
        static void GlfwWindowCharCallback(GLFWwindow *window, std::uint32_t codepoint);
        static void GlfwWindowMousePositionCallback(GLFWwindow *window, double x, double y);
        static void GlfwWindowMouseEnterCallback(GLFWwindow *window, std::int32_t entered);
        static void GlfwWindowMouseButtonCallback(GLFWwindow *window, std::int32_t button, std::int32_t action, std::int32_t mods);
        static void GlfwWindowMouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
        static void GlfwWindowDropCallback(GLFWwindow *window, std::int32_t count, const char **paths);
    };
}  // namespace vre