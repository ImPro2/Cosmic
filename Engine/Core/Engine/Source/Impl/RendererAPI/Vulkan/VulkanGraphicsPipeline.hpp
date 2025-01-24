#pragma once
#include "VulkanDebugUtils.hpp"
#include "VulkanDevice.hpp"
#include "VulkanRenderPass.hpp"

#include "Renderer/GraphicsPipeline.hpp"

namespace Cosmic
{

	class VulkanRenderPass;
	class VulkanDevice;

	class VulkanGraphicsPipeline : public GraphicsPipeline
	{
	public:
		VulkanGraphicsPipeline(const GraphicsPipelineInfo& info, const Ref<VulkanRenderPass>& renderPass, const Ref<VulkanDevice>& device);
		~VulkanGraphicsPipeline();

	public:
		void       CreateHandle();
		VkPipeline GetOrCreateHandle();

	public:
		VkPipeline       GetHandle()               const { return mPipelineHandle;       }
		VkPipelineLayout GetPipelineLayoutHandle() const { return mPipelineLayoutHandle; }

	private:
		VkPipeline       mPipelineHandle;
		VkPipelineLayout mPipelineLayoutHandle;

		Ref<VulkanRenderPass> mRenderPass;
		Ref<VulkanDevice>     mDevice;
	};

}
