#include <VREngine/Window.hpp>

namespace vre {
    void Window::GlfwErrorCallback(std::int32_t errorCode, const char *description) {
        std::string errorName{};

        switch (errorCode) {
            case GLFW_NO_ERROR:
                errorName = "GLFW_NO_ERROR";
                break;
            case GLFW_NOT_INITIALIZED:
                errorName = "GLFW_NOT_INITIALIZED";
                break;
            case GLFW_NO_CURRENT_CONTEXT:
                errorName = "GLFW_NO_CURRENT_CONTEXT";
                break;
            case GLFW_INVALID_ENUM:
                errorName = "GLFW_INVALID_ENUM";
                break;
            case GLFW_INVALID_VALUE:
                errorName = "GLFW_INVALID_VALUE";
                break;
            case GLFW_OUT_OF_MEMORY:
                errorName = "GLFW_OUT_OF_MEMORY";
                break;
            case GLFW_API_UNAVAILABLE:
                errorName = "GLFW_API_UNAVAILABLE";
                break;
            case GLFW_VERSION_UNAVAILABLE:
                errorName = "GLFW_VERSION_UNAVAILABLE";
                break;
            case GLFW_PLATFORM_ERROR:
                errorName = "GLFW_PLATFORM_ERROR";
                break;
            case GLFW_FORMAT_UNAVAILABLE:
                errorName = "GLFW_FORMAT_UNAVAILABLE";
                break;
            case GLFW_NO_WINDOW_CONTEXT:
                errorName = "GLFW_NO_WINDOW_CONTEXT";
                break;
            default:
                errorName = "GLFW_UNKNOWN_ERROR";
                break;
        }

        LOG_ERROR("Glfw Error ({}): '{}'", errorName, description);
    }

    void Window::GlfwWindowCloseCallback(GLFWwindow *window) {
        EventObserver::Process(WindowCloseEvent{});
    }

    void Window::GlfwWindowSizeCallback(GLFWwindow *window, std::int32_t width, std::int32_t height) {
        EventObserver::Process(WindowResizeEvent{static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height)});
    }

    void Window::GlfwWindowContentScaleCallback(GLFWwindow *window, float xScale, float yScale) {
        EventObserver::Process(WindowContentScaleEvent{xScale, yScale});
    }

    void Window::GlfwWindowPositionCallback(GLFWwindow *window, std::int32_t x, std::int32_t y) {
        EventObserver::Process(WindowMoveEvent{static_cast<std::uint32_t>(x), static_cast<std::uint32_t>(y)});
    }

    void Window::GlfwWindowMaximizeCallback(GLFWwindow *window, std::int32_t isMaximized) {
        if (isMaximized)
            EventObserver::Process(WindowMaximizeEvent{});
        else
            EventObserver::Process(WindowRestoreEvent{});
    }

    void Window::GlfwWindowMinimizeCallback(GLFWwindow *window, std::int32_t isMinimized) {
        if (isMinimized)
            EventObserver::Process(WindowMinimizeEvent{});
        else
            EventObserver::Process(WindowRestoreEvent{});
    }

    void Window::GlfwWindowFocusCallback(GLFWwindow *window, std::int32_t focused) {
        if (focused == GLFW_TRUE)
            EventObserver::Process(WindowGainFocusEvent{});
        else
            EventObserver::Process(WindowLoseFocusEvent{});
    }

    void Window::GlfwWindowRefreshCallback(GLFWwindow *window) {
        EventObserver::Process(WindowRefreshEvent{});
    }

    void Window::GlfwWindowKeyCallback(GLFWwindow *window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods) {
        g_KeyMods = static_cast<KeyMod>(mods);
        if (action == GLFW_PRESS)
            g_Keys[key] = true;
        else if (action == GLFW_RELEASE)
            g_Keys[key] = false;

        EventObserver::Process(WindowKeyEvent{static_cast<KeyCode>(key), static_cast<KeyMod>(mods), static_cast<KeyAction>(action)});
    }

    void Window::GlfwWindowCharCallback(GLFWwindow *window, std::uint32_t codepoint) {
        EventObserver::Process(WindowCharEvent{codepoint});
    }

    void Window::GlfwWindowMousePositionCallback(GLFWwindow *window, double x, double y) {
        EventObserver::Process(WindowMouseMoveEvent{x, y});
    }

    void Window::GlfwWindowMouseEnterCallback(GLFWwindow *window, std::int32_t entered) {
        if (entered == GLFW_TRUE)
            EventObserver::Process(WindowMouseEnterEvent{});
        else
            EventObserver::Process(WindowMouseLeaveEvent{});
    }

    void Window::GlfwWindowMouseButtonCallback(GLFWwindow *window, std::int32_t button, std::int32_t action, std::int32_t mods) {
        g_KeyMods = static_cast<KeyMod>(mods);
        if (action == GLFW_PRESS)
            g_MouseButtons[button] = true;
        else if (action == GLFW_RELEASE)
            g_MouseButtons[button] = false;

        EventObserver::Process(WindowMouseButtonEvent{static_cast<MouseButton>(button), static_cast<KeyMod>(mods), static_cast<KeyAction>(action)});
    }

    void Window::GlfwWindowMouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
        EventObserver::Process(WindowMouseScrollEvent{xOffset, yOffset});
    }

    void Window::GlfwWindowDropCallback(GLFWwindow *window, std::int32_t count, const char **paths) {
        std::vector<std::string> event{};
        event.resize(count);
        for (std::size_t i = 0; i < count; i++) {
            event[i] = paths[i];
        }

        EventObserver::Process(WindowDropEvent{event});
    }
}  // namespace vre