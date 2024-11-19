#include "cspch.hpp"
#include "ImGuiUtil.hpp"

#include "App/Input.hpp"
#include "imgui.h"

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

    bool MenuItem(const char* label, const char* shortcutStr, const Vector<EKeyCode>& shortcut, bool* enabledPtr, bool enabled)
    {
        return ImGui::MenuItem(label, shortcutStr, enabledPtr, enabled) || Util::IsShortcutPressed(shortcut);
    }

    bool SelectionRect(ImVec2* startPos, ImVec2* endPos, ImGuiMouseButton mouseButton)
    {
        if (ImGui::IsMouseClicked(mouseButton))
            *startPos = ImGui::GetMousePos();

        if (ImGui::IsMouseDown(mouseButton))
        {
            *endPos = ImGui::GetMousePos();
            
            ImDrawList* drawList = ImGui::GetForegroundDrawList();
            drawList->AddRect(*startPos, *endPos, ImGui::GetColorU32(IM_COL32(0, 130, 216, 255)));
            drawList->AddRectFilled(*startPos, *endPos, ImGui::GetColorU32(IM_COL32(0, 130, 216, 50)));
        }

        return ImGui::IsMouseReleased(mouseButton);
    }

}
