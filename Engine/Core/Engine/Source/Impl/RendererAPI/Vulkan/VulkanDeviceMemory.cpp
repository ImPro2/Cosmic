#include "cspch.hpp"
#include "VulkanDeviceMemory.hpp"
#include "VulkanDevice.hpp"

namespace Cosmic
{

	namespace Utils
	{

		static uint32 FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDeviceHandle)
		{
			VkPhysicalDeviceMemoryProperties memoryProperties;
			vkGetPhysicalDeviceMemoryProperties(physicalDeviceHandle, &memoryProperties);

			for (uint32 i = 0; i < memoryProperties.memoryTypeCount; i++)
			{
				if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
				{
					return i;
				}
			}

			return 0;
		}

	}

	VulkanDeviceMemory::VulkanDeviceMemory(const Ref<VulkanDevice>& device, const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags memoryPropertyFlags)
		: mDevice(device), mMemoryRequirements(memoryRequirements), mMemoryPropertyFlags(memoryPropertyFlags)
	{
	}

	VulkanDeviceMemory::~VulkanDeviceMemory()
	{
		Free();
	}

	void VulkanDeviceMemory::Allocate()
	{
		VkMemoryAllocateInfo memoryAllocateInfo = {
			.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize  = mMemoryRequirements.size,
			.memoryTypeIndex = Utils::FindMemoryType(mMemoryRequirements.memoryTypeBits, mMemoryPropertyFlags, mDevice->GetPhysicalDevice()->GetHandle())
		};

		VK_CALL(vkAllocateMemory(mDevice->GetHandle(), &memoryAllocateInfo, nullptr, &mDeviceMemoryHandle));
	}

	void VulkanDeviceMemory::Free()
	{
		vkFreeMemory(mDevice->GetHandle(), mDeviceMemoryHandle, nullptr);
	}

	void VulkanDeviceMemory::Map(VkDeviceSize offset, VkDeviceSize size, void** data)
	{
		VK_CALL(vkMapMemory(mDevice->GetHandle(), mDeviceMemoryHandle, offset, size, 0, data));
	}

	void VulkanDeviceMemory::Unmap()
	{
		vkUnmapMemory(mDevice->GetHandle(), mDeviceMemoryHandle);
	}

	void VulkanDeviceMemory::CopyData(VkDeviceSize offset, VkDeviceSize size, const void* src)
	{
		void* data;
		Map(offset, size, &data);

		std::memcpy(data, src, size);

		Unmap();
	}

}