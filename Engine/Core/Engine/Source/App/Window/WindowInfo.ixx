module;
#include "cspch.hpp"
#include <functional>
export module Cosmic.App.WindowInfo;

import Cosmic.Base.Types;
import Cosmic.Base.Tuples;

namespace Cosmic
{

    export struct DesktopWindowInfo
    {
        uint2  Size;
        uint2  Position;
        String Title;
        bool   IsVSync;

        DesktopWindowInfo(
            uint2         size  = { 800, 600 },
            uint2         pos   = { 25,  25  },
            const String& title = "Cosmic Window",
            bool          vsync = false
        )
            : Size(size),
              Position(pos),
              Title(title),
              IsVSync(vsync)
        {
        }
    };

}