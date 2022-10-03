module;
#include "cspch.hpp"
export module Cosmic.App.Input;

import Cosmic.Base;
import Cosmic.App.KeyAndMouseCodes;

export import Cosmic.App.KeyAndMouseCodes;

namespace Cosmic
{

    export enum class ECursorMode
    {
        Normal,
        Hidden
    };

    export class Input
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