#include "cspch.hpp"
#include "VulkanSemaphore.hpp"
#include "VulkanDevice.hpp"

namespace Cosmic
{

	VulkanSemaphore::VulkanSemaphore(const Ref<VulkanDevice>& device, VkPipelineStageFlags pipelineStageFlags)
		: mDevice(device), mPipelineStageFlags(pipelineStageFlags)
	{
		VkSemaphoreCreateInfo semaphoreCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
		};

		VK_CALL(vkCreateSemaphore(mDevice->GetHandle(), &semaphoreCreateInfo, nullptr, &mSemaphoreHandle));
	}

	VulkanSemaphore::~VulkanSemaphore()
	{
		vkDestroySemaphore(mDevice->GetHandle(), mSemaphoreHandle, nullptr);
	}

}