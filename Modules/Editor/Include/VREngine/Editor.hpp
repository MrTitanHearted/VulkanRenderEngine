#pragma once

#include <VREngine/Engine.hpp>

namespace vre {
    class Editor {
       public:
        Editor();
        ~Editor();

        void init();
        void run();
        void release();

       private:
        bool m_IsInitialized;
        bool m_IsRunning;

        EventCallbackHandle m_WindowCloseCallbackHandle;
        EventCallbackHandle m_WindowKeyCallbackHandle;

       private:
        void closeCallback(const WindowCloseEvent &event);
        void keyCallback(const WindowKeyEvent &event);
    };
}  // namespace vre