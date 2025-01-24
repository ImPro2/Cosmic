#pragma once
#include "VulkanDebugUtils.hpp"

#undef max

namespace Cosmic
{

	class VulkanDevice;
	class VulkanCommandBuffer;
	class VulkanQueue;
	class VulkanFence;

	class VulkanCommandPool : public IRefCounted
	{
	public:
		VulkanCommandPool(const Ref<VulkanDevice>& device, uint32 queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
		~VulkanCommandPool();

	public:
		void Reset(VkCommandPoolResetFlags resetFlags);

		Ref<VulkanCommandBuffer> AllocateCommandBuffer(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		Vector<Ref<VulkanCommandBuffer>> AllocateCommandBuffers(uint32 count, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

	public:
		VkCommandPool            GetHandle()           const { return mCommandPoolHandle; }
		int32                    GetQueueFamilyIndex() const { return mQueueFamilyIndex;  }
		VkCommandPoolCreateFlags GetFlags()            const { return mFlags;             }

	private:
		VkCommandPool mCommandPoolHandle;

		int32 mQueueFamilyIndex;
		VkCommandPoolCreateFlags mFlags;
		Ref<VulkanDevice> mDevice;

		friend class VulkanCommandBuffer;
	};

}
