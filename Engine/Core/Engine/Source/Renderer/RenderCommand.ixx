module;
#include "cspch.hpp"
export module Cosmic.Renderer.RenderCommand;

import Cosmic.Base;
import Cosmic.Renderer.RendererAPI;

namespace Cosmic
{

    export class RenderCommand
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
