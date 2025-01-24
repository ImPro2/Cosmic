#include "cspch.hpp"
#include "RenderPass.hpp"

CS_MODULE_LOG_INFO(Cosmic, Renderer.RenderPass);

namespace Cosmic
{

	RenderPass::RenderPass(const String& name)
		: mName(name)
	{
	}

	void RenderPass::AddInputAttachment(const String& name)
	{
		CS_NOT_IMPLEMENTED();
	}

	void RenderPass::SetDepthStencilInput(const String& name)
	{
		CS_NOT_IMPLEMENTED();
	}

	void RenderPass::AddColorOutput(const AttachmentInfo& attachmentInfo)
	{
		mColorOutputs.push_back(attachmentInfo);
	}

	void RenderPass::SetDepthStencilOutput(const AttachmentInfo& attachmentInfo)
	{
		mDepthStencilOutput = attachmentInfo;
	}


}