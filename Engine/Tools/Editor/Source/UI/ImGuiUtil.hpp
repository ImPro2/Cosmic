#pragma once
#include "Base/Types.hpp"
#include "App/KeyAndMouseCodes.hpp"

#include <imgui.h>

namespace Cosmic::ImGuiUtil
{

    bool BeginMenu(const char* label, const Vector<EKeyCode>& shortcut, bool open = false);
    bool MenuItem(const char* label, const char* shortcutStr, const Vector<EKeyCode>& shortcut, bool* enabledPtr = nullptr, bool enabled = true);

    // startPos and endPos are screen coordinates relative to the window
    bool SelectionRect(ImVec2* startPos, ImVec2* endPos, ImGuiMouseButton mouseButton);

}
