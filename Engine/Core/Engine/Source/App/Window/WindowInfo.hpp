#pragma once
#include "Base/Types.hpp"
#include "Base/Tuples.hpp"

namespace Cosmic
{

    struct DesktopWindowInfo
    {
        uint2  Size;
        uint2  Position;
        String Title;
        bool   IsVSync;

        DesktopWindowInfo(
            uint2         size  = { 1280, 720 },
            uint2         pos   = { 25,   25  },
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