module;
#include "cspch.hpp"
module Cosmic.Renderer.RenderCommand;

import Cosmic.Base;
import Cosmic.Renderer.RendererAPI;

namespace Cosmic
{

    void RenderCommand::Init(ERendererAPI api)
    {
        CS_PROFILE_FN();

        sAPI = CreateRendererAPI(api);
        sAPI->Init();
    }

    void RenderCommand::Shutdown()
    {
        CS_PROFILE_FN();

        sAPI->Shutdown();
    }

    void RenderCommand::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
    {
        CS_PROFILE_FN();

        sAPI->SetViewport(x, y, width, height);
    }

    void RenderCommand::SetClearColor(float4 color)
    {
        CS_PROFILE_FN();

        sAPI->SetClearColor(color);
    }

    void RenderCommand::Clear()
    {
        CS_PROFILE_FN();

        sAPI->Clear();
    }

    void RenderCommand::Render(EPrimitiveTopology primitiveTopology, uint32 count)
    {
        CS_PROFILE_FN();

        sAPI->Render(primitiveTopology, count);
    }

}
