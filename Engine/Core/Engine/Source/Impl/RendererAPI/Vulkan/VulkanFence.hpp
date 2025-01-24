#pragma once
#include "VulkanDebugUtils.hpp"
#include "VulkanDevice.hpp"

#undef max

namespace Cosmic
{

	class VulkanFence : public IRefCounted
	{
	public:
		VulkanFence(const Ref<VulkanDevice>& device, bool signalled = false);
		~VulkanFence();

	public:
		void Reset();
		void Wait(uint64 timeout = std::numeric_limits<uint64>::max()) const;
		bool IsSignalled() const;

	public:
		VkFence GetHandle() const { return mFenceHandle; }

		Vector<Ref<VulkanSemaphore>>& GetWaitSemaphores() { return mWaitSemaphores; }

	private:
		VkFence mFenceHandle = VK_NULL_HANDLE;

		Vector<Ref<VulkanSemaphore>> mWaitSemaphores;

		Ref<VulkanDevice> mDevice;
	};

}
