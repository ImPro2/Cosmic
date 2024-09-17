#pragma once

#include "Base/Base.hpp"
#include "App/KeyAndMouseCodes.hpp"

namespace Cosmic
{

    enum class ECursorMode
    {
        Normal,
        Hidden
    };

    class Input
    {
    public:
        static bool       IsKeyPressed(EKeyCode code);
        static bool       IsMouseButtonPressed(EMouseCode button);

        static EKeyCode   GetKeyPressed();
        static EMouseCode GetMouseButtonPressed();

        static float2     GetMousePosition();
        static void       SetCursorMode(ECursorMode mode);
    };

}