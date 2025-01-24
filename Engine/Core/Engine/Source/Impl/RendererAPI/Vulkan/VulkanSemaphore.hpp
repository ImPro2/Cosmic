#pragma once
#include "VulkanDebugUtils.hpp"

namespace Cosmic
{

	class VulkanDevice;

	class VulkanSemaphore : public IRefCounted
	{
	public:
		VulkanSemaphore(const Ref<VulkanDevice>& device, VkPipelineStageFlags pipelineStageFlags);
		~VulkanSemaphore();

	public:
		VkSemaphore          GetHandle()             const { return mSemaphoreHandle;    }
		VkPipelineStageFlags GetPipelineStageFlags() const { return mPipelineStageFlags; }

	private:
		VkSemaphore          mSemaphoreHandle;
		VkPipelineStageFlags mPipelineStageFlags;

		Ref<VulkanDevice> mDevice;
	};

}
