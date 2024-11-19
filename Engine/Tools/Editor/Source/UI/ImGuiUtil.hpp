#pragma once
#include "Base/Types.hpp"
#include "App/KeyAndMouseCodes.hpp"

#include <imgui.h>

namespace Cosmic::ImGuiUtil
{

    bool BeginMenu(const char* label, const Vector<EKeyCode>& shortcut);
    bool MenuItem(const char* label, const char* shortcutStr, const Vector<EKeyCode>& shortcut, bool* enabledPtr = nullptr);

}
