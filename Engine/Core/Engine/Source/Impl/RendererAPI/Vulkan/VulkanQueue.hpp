#pragma once
#include "VulkanDebugUtils.hpp"

namespace Cosmic
{

	class VulkanDevice;
	class VulkanSemaphore;
	class VulkanFence;
	class VulkanCommandBuffer;
	class VulkanSwapchain;
	class VulkanCommandPool;

	class VulkanQueue : public IRefCounted
	{
	public:
		VulkanQueue(const Ref<VulkanDevice>& device, VkQueue queueHandle, VkQueueFlags flags, int32 queueFamilyIndex, int32 queueIndex);
		~VulkanQueue();

	public:
		void WaitIdle();

		VkResult Submit(const Vector<Ref<VulkanCommandBuffer>>& commandBuffers, const Vector<Ref<VulkanSemaphore>>& waitSemaphores, const Vector<Ref<VulkanSemaphore>>& signalSemaphores, const Ref<VulkanFence>& signalFence);
		VkResult Present(const Ref<VulkanSwapchain>& swapchain, const Vector<Ref<VulkanSemaphore>>& waitSemaphores);

	public:
		void SubmitSingleTimeCommands(const Ref<VulkanCommandPool>& commandPool, const Ref<VulkanFence>& signalFence, std::function<void(const Ref<VulkanCommandBuffer>&)> function, uint64 timeout = std::numeric_limits<uint64>::max());

	public:
		VkQueue      GetHandle()           const { return mQueueHandle;      }
		VkQueueFlags GetFlags()            const { return mQueueFlags;       }
		int32        GetQueueFamilyIndex() const { return mQueueFamilyIndex; }
		int32        GetQueueIndex()       const { return mQueueIndex;       }

	private:
		VkQueue      mQueueHandle;
		VkQueueFlags mQueueFlags;
		int32        mQueueFamilyIndex;
		int32        mQueueIndex;

		Ref<VulkanDevice> mDevice;

		friend class VulkanDevice;
	};

}
