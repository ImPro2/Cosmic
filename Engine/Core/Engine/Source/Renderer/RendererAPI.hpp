#pragma once
#include "Base/Base.hpp"
#include "Memory/Memory.hpp"
#include "Renderer/GraphicsPipeline.hpp"

namespace Cosmic
{

    enum class ERendererAPI
    {
        None    = -1,
        OpenGL  =  0,
        DirectX =  1,
        Vulkan  =  2,
        Metal   =  3
    };

    class RendererAPI : public IRefCounted
    {
    public:
        RendererAPI(ERendererAPI api);
        ~RendererAPI();

        virtual void Init()                                                       = 0;
        virtual void Shutdown()                                                   = 0;
        virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
        virtual void SetClearColor(float4 color)                                  = 0;
        virtual void Clear()                                                      = 0;
        virtual void Render(EPrimitiveTopology primitiveTopology, uint32 count)   = 0;

        static ERendererAPI Get() { return sAPI; }

    private:
        inline static ERendererAPI sAPI;
    };

    ERendererAPI               PlatformNativeRendererAPI();
    PersistentRef<RendererAPI> CreateRendererAPI(ERendererAPI api);

}
