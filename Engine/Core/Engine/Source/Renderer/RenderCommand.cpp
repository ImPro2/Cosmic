#include "cspch.hpp"
#include "RenderCommand.hpp"

#include "Base/Base.hpp"
#include "Renderer/RendererAPI.hpp"

namespace Cosmic
{

    Ref<RendererAPI> RenderCommand::Init(ERendererAPI api)
    {
        CS_PROFILE_FN();

        sAPI = CreateRendererAPI(api);
        sAPI->Init();

        return sAPI;
    }

    void RenderCommand::Shutdown()
    {
        CS_PROFILE_FN();

        sAPI->Shutdown();
        sAPI.Release();
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
