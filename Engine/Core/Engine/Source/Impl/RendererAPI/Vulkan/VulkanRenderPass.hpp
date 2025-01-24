#pragma once
#include "VulkanDebugUtils.hpp"

#include "Renderer/RenderPass.hpp"

namespace Cosmic
{

	class VulkanSwapchain;
	class VulkanDevice;

	class VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass(const String& name, const Ref<VulkanSwapchain>& swapchain, const Ref<VulkanDevice>& device);
		~VulkanRenderPass();

		void         CreateHandle();
		VkRenderPass GetOrCreateHandle();

	public:
		VkRenderPass GetHandle() const { return mRenderPassHandle; }

	private:
		VkRenderPass mRenderPassHandle = VK_NULL_HANDLE;

		Ref<VulkanSwapchain> mSwapchain;
		Ref<VulkanDevice>    mDevice;
	};

}
