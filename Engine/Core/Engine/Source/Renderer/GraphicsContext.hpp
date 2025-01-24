#pragma once
#include "Base/Base.hpp"
#include "Memory/Memory.hpp"

namespace Cosmic
{

    struct RendererInfo
    {
        String Vendor;
        String Renderer;
        String Version;
    };

    // One context per window.
    class GraphicsContext : public IRefCounted
    {
    public:
        virtual void  Init()        = 0;
        virtual void  Shutdown()    = 0;
        virtual void  Present()     = 0;

        RendererInfo  GetInfo()     { return mInfo;          }
        const String& GetVendor()   { return mInfo.Vendor;   }
        const String& GetRenderer() { return mInfo.Renderer; }
        const String& GetVersion()  { return mInfo.Version;  }

    protected:
        RendererInfo mInfo;
    };

    Ref<GraphicsContext> CreateGraphicsContext(void* window);

}
