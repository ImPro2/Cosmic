module;
#include "cspch.hpp"
module Cosmic.Renderer.RenderCommand;

import Cosmic.Base;
import Cosmic.Renderer.RendererAPI;

namespace Cosmic
{

    void RenderCommand::Init(ERendererAPI api)
    {
        sAPI = CreateRendererAPI(api);
        sAPI->Init();
    }

    void RenderCommand::Shutdown()
    {
        sAPI->Shutdown();
    }

    void RenderCommand::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
    {
        sAPI->SetViewport(x, y, width, height);
    }

    void RenderCommand::SetClearColor(float4 color)
    {
        sAPI->SetClearColor(color);
    }

    void RenderCommand::Clear()
    {
        sAPI->Clear();
    }

    void RenderCommand::Render(EPrimitiveTopology primitiveTopology, uint32 count)
    {
        sAPI->Render(primitiveTopology, count);
    }

}