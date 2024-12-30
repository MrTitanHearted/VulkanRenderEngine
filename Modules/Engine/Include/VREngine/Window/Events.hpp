#pragma once

#include <VREngine/Core.hpp>
#include <VREngine/Window/Enums.hpp>

namespace vre {
    class WindowCloseEvent : public IEvent {
       public:
        WindowCloseEvent() = default;
    };

    class WindowResizeEvent : public IEvent {
       public:
        std::uint32_t Width;
        std::uint32_t Height;

        WindowResizeEvent(std::uint32_t width, std::uint32_t height)
            : Width{width}, Height{height} {}
    };

    class WindowContentScaleEvent : public IEvent {
       public:
        float XScale;
        float YScale;

        WindowContentScaleEvent(float xScale, float yScale)
            : XScale{xScale}, YScale{yScale} {}
    };

    class WindowMoveEvent : public IEvent {
       public:
        std::uint32_t X;
        std::uint32_t Y;

        WindowMoveEvent(std::uint32_t x, std::uint32_t y) : X{x}, Y{y} {}
    };

    class WindowRestoreEvent : public IEvent {
       public:
        WindowRestoreEvent() = default;
    };

    class WindowMinimizeEvent : public IEvent {
       public:
        WindowMinimizeEvent() = default;
    };

    class WindowMaximizeEvent : public IEvent {
       public:
        WindowMaximizeEvent() = default;
    };

    class WindowGainFocusEvent : public IEvent {
       public:
        WindowGainFocusEvent() = default;
    };

    class WindowLoseFocusEvent : public IEvent {
       public:
        WindowLoseFocusEvent() = default;
    };

    class WindowRefreshEvent : public IEvent {
       public:
        WindowRefreshEvent() = default;
    };

    class WindowKeyEvent : public IEvent {
       public:
        KeyCode   Key;
        KeyMod    Mods;
        KeyAction Action;

        WindowKeyEvent(KeyCode key, KeyMod mods, KeyAction action)
            : Key{key}, Mods{mods}, Action{action} {}
    };

    class WindowCharEvent : public IEvent {
       public:
        std::uint32_t Codepoint;

        WindowCharEvent(std::uint32_t codepoint) : Codepoint{codepoint} {}
    };

    class WindowMouseMoveEvent : public IEvent {
       public:
        double X;
        double Y;

        WindowMouseMoveEvent(double x, double y) : X{x}, Y{y} {}
    };

    class WindowMouseEnterEvent : public IEvent {
       public:
        WindowMouseEnterEvent() = default;
    };

    class WindowMouseLeaveEvent : public IEvent {
       public:
        WindowMouseLeaveEvent() = default;
    };

    class WindowMouseButtonEvent : public IEvent {
       public:
        MouseButton Button;
        KeyMod      Mods;
        KeyAction   Action;

        WindowMouseButtonEvent(MouseButton button, KeyMod mods, KeyAction action)
            : Button{button}, Mods{mods}, Action{action} {}
    };

    class WindowMouseScrollEvent : public IEvent {
       public:
        double XOffset;
        double YOffset;

        WindowMouseScrollEvent(double x, double y) : XOffset{x}, YOffset{y} {}
    };

    class WindowDropEvent : public IEvent {
       public:
        std::vector<std::string> Paths;

        WindowDropEvent(const std::vector<std::string> &paths) : Paths{paths} {}
    };
}  // namespace vre