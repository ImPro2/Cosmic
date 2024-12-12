#pragma once
#include "Base/Base.hpp"
#include "Renderer/RendererAPI.hpp"
#include "Memory/ScopedPtr.hpp"

namespace Cosmic
{

    class RenderCommand
    {
    public:
        static void Init(ERendererAPI api);
        static void Shutdown();
        static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);
        static void SetClearColor(float4 color);
        static void Clear();
        static void Render(EPrimitiveTopology primitiveTopology, uint32 count);

    private:
        inline static Scope<RendererAPI> sAPI;
    };

}
