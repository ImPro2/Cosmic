#pragma once
#include "VulkanDebugUtils.hpp"
#include "VulkanInstance.hpp"
#include "VulkanPhysicalDevice.hpp"
#include "VulkanQueue.hpp"

namespace Cosmic
{

	struct VulkanQueueCreateInfo
	{
		int32           QueueFamilyIndex;
		Vector<float32> QueuePriorities;
	};

	Vector<VkExtensionProperties> VulkanGetSupportedDeviceExtensionProperties(VkPhysicalDevice physicalDeviceHandle, const char* layerName = nullptr);

	class VulkanDevice : public IRefCounted
	{
	public:
		VulkanDevice(const Vector<VulkanQueueCreateInfo>& queues, const Ref<VulkanPhysicalDevice>& physicalDevice, const VkPhysicalDeviceFeatures& physicalDeviceFeatures, const Vector<const char*>& requiredDeviceExtensionNames);
		~VulkanDevice();

	public:
		VkDevice GetHandle() const { return mDeviceHandle; }

		Ref<VulkanPhysicalDevice>        GetPhysicalDevice()       { return mPhysicalDevice; }
		const Ref<VulkanPhysicalDevice>& GetPhysicalDevice() const { return mPhysicalDevice; }

		Vector<Ref<VulkanQueue>>&       GetQueues()       { return mQueues; }
		const Vector<Ref<VulkanQueue>>& GetQueues() const { return mQueues; }

		const Vector<VkExtensionProperties>& GetExtensions() const { return mDeviceExtensions; }

	public:
		Ref<VulkanQueue> GetQueue(int32 queueFamilyIndex, uint32 queueIndex = 0);

	private:
		Vector<VkExtensionProperties> FilterSupportedDeviceExtensions(const Vector<const char*>& deviceExtensionNames);

		Vector<VkDeviceQueueCreateInfo> GetVkDeviceQueueCreateInfos(const Vector<VulkanQueueCreateInfo>& queues);
		void CreateDeviceHandle(const Vector<VkDeviceQueueCreateInfo>& deviceQueueCreateInfos);
		void CreateQueues(const Vector<VkDeviceQueueCreateInfo>& deviceQueueCreateInfos);

	private:
		VkDevice mDeviceHandle;

		Ref<VulkanPhysicalDevice>     mPhysicalDevice;
		Vector<Ref<VulkanQueue>>      mQueues;
		VkPhysicalDeviceFeatures      mEnabledFeatures;
		Vector<VkExtensionProperties> mDeviceExtensions;
	};

}
