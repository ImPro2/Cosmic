module;
#include "cspch.hpp"
export module Cosmic.Renderer.GraphicsContext;

import Cosmic.Base;

namespace Cosmic
{

    export struct RendererInfo
    {
        String Vendor;
        String Renderer;
        String Version;
    };

    // One context per window.
    export class GraphicsContext
    {
    public:
        virtual void  Init()        = 0;
        virtual void  SwapBuffers() = 0;

        RendererInfo  GetInfo()     { return mInfo;          }
        const String& GetVendor()   { return mInfo.Vendor;   }
        const String& GetRenderer() { return mInfo.Renderer; }
        const String& GetVersion()  { return mInfo.Version;  }

    protected:
        RendererInfo mInfo;
    };

    export Scope<GraphicsContext> CreateGraphicsContext(void* window);

}
