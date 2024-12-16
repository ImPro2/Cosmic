#pragma once
#include "Base/Base.hpp"
#include "Renderer/RendererAPI.hpp"
#include "Memory/SmartPtrs.hpp"

namespace Cosmic
{

    class Application;

    class RenderCommand
    {
    private:
        static Ref<RendererAPI> Init(ERendererAPI api);
        static void             Shutdown();

    public:
        static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);
        static void SetClearColor(float4 color);
        static void Clear();
        static void Render(EPrimitiveTopology primitiveTopology, uint32 count);

    private:
        inline static Ref<RendererAPI> sAPI;

        friend class Application;
    };

}
