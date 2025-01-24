#include "cspch.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanDevice.hpp"

namespace Cosmic
{

	VulkanBuffer::VulkanBuffer(const Ref<VulkanDevice>& device, uint32 size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags)
		: mDevice(device), mUsageFlags(usage)
	{
		VkBufferCreateInfo bufferCreateInfo = {
			.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.flags       = 0,
			.size        = size,
			.usage       = mUsageFlags,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		};

		VK_CALL(vkCreateBuffer(mDevice->GetHandle(), &bufferCreateInfo, nullptr, &mBufferHandle));

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(mDevice->GetHandle(), mBufferHandle, &memoryRequirements);

		mDeviceMemory = CreateRef<VulkanDeviceMemory>(mDevice, memoryRequirements, memoryPropertyFlags);
		mDeviceMemory->Allocate();

		VK_CALL(vkBindBufferMemory(mDevice->GetHandle(), mBufferHandle, mDeviceMemory->GetHandle(), 0));
	}

	VulkanBuffer::~VulkanBuffer()
	{
		vkDestroyBuffer(mDevice->GetHandle(), mBufferHandle, nullptr);
	}

}