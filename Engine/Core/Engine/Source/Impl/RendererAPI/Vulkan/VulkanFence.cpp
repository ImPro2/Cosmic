#include "cspch.hpp"
#include "VulkanFence.hpp"
#include "VulkanSemaphore.hpp"

CS_MODULE_LOG_INFO(Cosmic, Impl.RendererAPI.Vulkan.VulkanFence);

namespace Cosmic
{

	VulkanFence::VulkanFence(const Ref<VulkanDevice>& device, bool signalled)
		: mDevice(device)
	{
		VkFenceCreateInfo fenceCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = signalled ? VK_FENCE_CREATE_SIGNALED_BIT : (VkFenceCreateFlags)0
		};

		VK_CALL(vkCreateFence(mDevice->GetHandle(), &fenceCreateInfo, nullptr, &mFenceHandle));
	}

	VulkanFence::~VulkanFence()
	{
		vkDestroyFence(mDevice->GetHandle(), mFenceHandle, nullptr);
	}

	void VulkanFence::Reset()
	{
		VK_CALL(vkResetFences(mDevice->GetHandle(), 1, &mFenceHandle));
	}

	void VulkanFence::Wait(uint64 timeout /*= std::numeric_limits<uint64>::max()*/) const
	{
		VK_CALL(vkWaitForFences(mDevice->GetHandle(), 1, &mFenceHandle, VK_TRUE, timeout));
	}

	bool VulkanFence::IsSignalled() const
	{
		VkResult status = vkGetFenceStatus(mDevice->GetHandle(), mFenceHandle);

		if (status == VK_SUCCESS)
			return true;
		else if (status == VK_NOT_READY)
			return false;

		CS_ASSERT(false, "Invalid fence state");
		return false;
	}

}