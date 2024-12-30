#include <VREngine/Editor.hpp>

namespace vre {
    Editor::Editor()
        : m_IsInitialized{false} {}

    Editor::~Editor() {
        VRE_ASSERT(!m_IsInitialized, "vre::Editor must be released before closing");
    }

    void Editor::init() {
        DVRE_ASSERT(!m_IsInitialized, "vre::Editor must be released before initialized");
        DLOG_INFO("Initializing vre::Editor");

        m_WindowCloseCallbackHandle = EventObserver::AddCallback<WindowCloseEvent>(this, &Editor::closeCallback);
        m_WindowKeyCallbackHandle   = EventObserver::AddCallback<WindowKeyEvent>(this, &Editor::keyCallback);

        m_IsInitialized = true;
    }

    void Editor::run() {
        DVRE_ASSERT(m_IsInitialized, "vre::Editor must be initialized");

        m_IsRunning = true;

        while (m_IsRunning) {
            Window::PollEvents();
        }
    }

    void Editor::release() {
        DVRE_ASSERT(m_IsInitialized, "vre::Editor must be initialized before releasing");
        DLOG_INFO("Releasing vre::Editor");

        EventObserver::RemoveCallback<WindowCloseEvent>(m_WindowCloseCallbackHandle);
        EventObserver::RemoveCallback<WindowKeyEvent>(m_WindowKeyCallbackHandle);

        m_IsInitialized = false;
    }

    void Editor::closeCallback(const WindowCloseEvent &event) {
        m_IsRunning = false;
    }

    void Editor::keyCallback(const WindowKeyEvent &event) {
        if (event.Action != KeyAction::ePress) return;

        if (event.Key == KeyCode::eF)
            if (Window::IsFullscreen())
                Window::MakeWindowed();
            else
                Window::MakeFullscreen();
    }
}  // namespace vre