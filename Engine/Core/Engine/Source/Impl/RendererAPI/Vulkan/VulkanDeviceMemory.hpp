#pragma once
#include "VulkanDebugUtils.hpp"

namespace Cosmic
{

	class VulkanDevice;

	class VulkanDeviceMemory : public IRefCounted
	{
	public:
		VulkanDeviceMemory(const Ref<VulkanDevice>& device, const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags memoryPropertyFlags);
		~VulkanDeviceMemory();

	public:
		void Allocate();
		void Free();

		void Map(VkDeviceSize offset, VkDeviceSize size, void** data);
		void Unmap();

		void CopyData(VkDeviceSize offset, VkDeviceSize size, const void* src);

	public:
		VkDeviceMemory        GetHandle()              const { return mDeviceMemoryHandle;  }
		VkMemoryRequirements  GetMemoryRequirements()  const { return mMemoryRequirements;  }
		VkMemoryPropertyFlags GetMemoryPropertyFlags() const { return mMemoryPropertyFlags; }

	private:
		VkDeviceMemory        mDeviceMemoryHandle;
		VkMemoryRequirements  mMemoryRequirements;
		VkMemoryPropertyFlags mMemoryPropertyFlags;

		Ref<VulkanDevice> mDevice;
	};

}
