#pragma once
#include "VulkanDebugUtils.hpp"

#include "Renderer/RendererAPI.hpp"

namespace Cosmic
{

	class VulkanRendererAPI : public RendererAPI
	{
    public:
        virtual void Init()                                                       override;
        virtual void Shutdown()                                                   override;
        virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
        virtual void SetClearColor(float4 color)                                  override;
        virtual void Clear()                                                      override;
        virtual void Render(EPrimitiveTopology primitiveTopology, uint32 count)   override;
	};

}
