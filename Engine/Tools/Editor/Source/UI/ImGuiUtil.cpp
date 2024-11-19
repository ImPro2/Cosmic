#include "cspch.hpp"
#include "ImGuiUtil.hpp"

#include "App/Input.hpp"

namespace Cosmic::ImGuiUtil
{

    namespace Util
    {

        static bool IsShortcutPressed(const Vector<EKeyCode>& shortcut)
        {
            if (shortcut.empty())
                return false;

            bool shortcutPressed = true;

            for (EKeyCode key : shortcut)
                shortcutPressed = shortcutPressed && Input::IsKeyPressed(key);

            return shortcutPressed;
        }

    }

    bool BeginMenu(const char* label, const Vector<EKeyCode>& shortcut)
    {
        if (Util::IsShortcutPressed(shortcut))
            ImGui::OpenPopup(label);

        return ImGui::BeginMenu(label);
    }

    bool MenuItem(const char* label, const char* shortcutStr, const Vector<EKeyCode>& shortcut, bool* enabledPtr)
    {
        return ImGui::MenuItem(label, shortcutStr, enabledPtr) || Util::IsShortcutPressed(shortcut);
    }

}
