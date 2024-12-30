#include <VREngine/Window/Enums.hpp>

namespace vre {
    const char *GetKeyCodeName(KeyCode key) {
        const char *keyName = glfwGetKeyName(static_cast<std::int32_t>(key), 0);
        if (keyName != nullptr) return keyName;
        switch (key) {
            case KeyCode::eEscape:
                return "Escape";
            case KeyCode::eDelete:
                return "Delete";
            case KeyCode::eLeftAlt:
                return "Left Alt";
            case KeyCode::eRightAlt:
                return "Right Alt";
            case KeyCode::eLeftControl:
                return "Left Ctrl";
            case KeyCode::eRightControl:
                return "Right Ctrl";
            case KeyCode::eLeftShift:
                return "Left Shift";
            case KeyCode::eRightShift:
                return "Right Shift";
            case KeyCode::eLeftSuper:
                return "Left Super";
            case KeyCode::eRightSuper:
                return "Right Super";
            case KeyCode::eCapsLock:
                return "Caps Lock";
            case KeyCode::eTab:
                return "Tab";
            case KeyCode::eBackspace:
                return "Backspace";
            case KeyCode::eLeft:
                return "Left Arrow";
            case KeyCode::eDown:
                return "Down Arrow";
            case KeyCode::eUp:
                return "Up Arrow";
            case KeyCode::eRight:
                return "Right Arrow";
            case KeyCode::eNumLock:
                return "Num Lock";
            case KeyCode::ePrintScreen:
                return "Print Screen";
            case KeyCode::eScrollLock:
                return "Scroll Lock";
            case KeyCode::ePause:
                return "Pause";
            case KeyCode::eHome:
                return "Home";
            case KeyCode::eEnd:
                return "End";
            case KeyCode::ePageUp:
                return "Page Up";
            case KeyCode::ePageDown:
                return "Page Down";
            case KeyCode::eSpace:
                return "Space";
            case KeyCode::eEnter:
                return "Enter";
            case KeyCode::eKpEnter:
                return "Keypad Enter";
            default:
                return "Unknown Key";
        }
    }

    const char *GetKeyModName(KeyMod mod) {
        if (mod == KeyMod::eNone) {
            return "None";
        } else if (mod == KeyMod::eAll) {
            return "All";
        }

        static std::string name;
        name.clear();
        if (static_cast<std::uint32_t>(mod) & static_cast<std::uint32_t>(KeyMod::eShift)) {
            name += "Shift";
        }
        if (static_cast<std::uint32_t>(mod) & static_cast<std::uint32_t>(KeyMod::eControl)) {
            if (!name.empty()) name += " | ";
            name += "Ctrl";
        }
        if (static_cast<std::uint32_t>(mod) & static_cast<std::uint32_t>(KeyMod::eAlt)) {
            if (!name.empty()) name += " | ";
            name += "Alt";
        }
        if (static_cast<std::uint32_t>(mod) & static_cast<std::uint32_t>(KeyMod::eSuper)) {
            if (!name.empty()) name += " | ";
            name += "Super";
        }
        if (static_cast<std::uint32_t>(mod) & static_cast<std::uint32_t>(KeyMod::eCapsLock)) {
            if (!name.empty()) name += " | ";
            name += "Caps Lock";
        }
        if (static_cast<std::uint32_t>(mod) & static_cast<std::uint32_t>(KeyMod::eNumLock)) {
            if (!name.empty()) name += " | ";
            name += "Num Lock";
        }
        return &name[0];
    }

    const char *GetKeyActionName(KeyAction action) {
        switch (action) {
            case KeyAction::ePress:
                return "Press";
            case KeyAction::eRelease:
                return "Release";
            case KeyAction::eRepeat:
                return "Repeat";
            default:
                return "Unknown";
        }
    }

    const char *GetMouseButtonName(MouseButton button) {
        switch (button) {
            case MouseButton::eLeft:
                return "Left";
            case MouseButton::eRight:
                return "Right";
            case MouseButton::eMiddle:
                return "Middle";
            default:
                static std::array<char, 16> buttonName;
                std::fill(buttonName.begin(), buttonName.end(), '\0');
                std::uint32_t buttonFmt = static_cast<std::int32_t>(button);
                std::vformat_to(&buttonName[0], "X{}", std::make_format_args(buttonFmt));
                return &buttonName[0];
        }
    }

    const char *GetCursorModeName(CursorMode mode) {
        switch (mode) {
            case CursorMode::eNormal:
                return "Normal";
            case CursorMode::eHidden:
                return "Hidden";
            case CursorMode::eDisabled:
                return "Disabled";
            case CursorMode::eCaptured:
                return "Captured";
            default:
                return "Unknown";
        }
    }
}  // namespace vre