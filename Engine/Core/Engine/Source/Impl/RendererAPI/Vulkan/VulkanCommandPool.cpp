#include "cspch.hpp"
#include "VulkanCommandPool.hpp"

#include "VulkanDevice.hpp"
#include "VulkanCommandBuffer.hpp"
#include "VulkanQueue.hpp"
#include "VulkanFence.hpp"

namespace Cosmic
{

	VulkanCommandPool::VulkanCommandPool(const Ref<VulkanDevice>& device, uint32 queueFamilyIndex, VkCommandPoolCreateFlags flags /*= 0*/)
		: mDevice(device), mQueueFamilyIndex(queueFamilyIndex), mFlags(flags)
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo = {
			.sType             = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags            = mFlags,
			.queueFamilyIndex = (uint32)mQueueFamilyIndex
		};

		VK_CALL(vkCreateCommandPool(mDevice->GetHandle(), &commandPoolCreateInfo, nullptr, &mCommandPoolHandle));
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		vkDestroyCommandPool(mDevice->GetHandle(), mCommandPoolHandle, nullptr);
	}

	void VulkanCommandPool::Reset(VkCommandPoolResetFlags resetFlags)
	{
		vkResetCommandPool(mDevice->GetHandle(), mCommandPoolHandle, resetFlags);
	}

	Ref<VulkanCommandBuffer> VulkanCommandPool::AllocateCommandBuffer(VkCommandBufferLevel level)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
			.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool        = mCommandPoolHandle,
			.level              = level,
			.commandBufferCount = 1,
		};

		VkCommandBuffer commandBufferHandle;
		VK_CALL(vkAllocateCommandBuffers(mDevice->GetHandle(), &commandBufferAllocateInfo, &commandBufferHandle));

		return CreateRef<VulkanCommandBuffer>(mDevice, this, commandBufferHandle, level);
	}

	Vector<Ref<VulkanCommandBuffer>> VulkanCommandPool::AllocateCommandBuffers(uint32 count, VkCommandBufferLevel level)
	{
		VkCommandBuffer* commandBufferHandles = (VkCommandBuffer*)alloca(count * sizeof(VkCommandBuffer));

		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
			.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool        = mCommandPoolHandle,
			.level              = level,
			.commandBufferCount = count,
		};

		VK_CALL(vkAllocateCommandBuffers(mDevice->GetHandle(), &commandBufferAllocateInfo, commandBufferHandles));

		Vector<Ref<VulkanCommandBuffer>> commandBuffers(count);

		for (uint32 i = 0; i < commandBuffers.size(); i++)
		{
			commandBuffers[i] = CreateRef<VulkanCommandBuffer>(mDevice, this, commandBufferHandles[i], level);
		}

		return commandBuffers;

	}

}