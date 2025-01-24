#pragma once
#include "VulkanDebugUtils.hpp"
#include "VulkanStructs.hpp"

namespace Cosmic
{

	class VulkanInstance;
	class VulkanSurface;

	class VulkanPhysicalDevice : public IRefCounted
	{
	public:
		VulkanPhysicalDevice(const Ref<VulkanInstance>& instance, VkPhysicalDevice physicalDeviceHandle);
		~VulkanPhysicalDevice();

	public:
		bool SupportsDeviceExtension(const char* extensionName)                  const;
		bool SupportsDeviceExtensions(const Vector<const char*>& extensionNames) const;
		bool SupportsDeviceFeatures(const VkPhysicalDeviceFeatures& features)    const;

		QueueFamilyIndices GetQueueFamilyIndices(VkQueueFlags flags, const Ref<VulkanSurface>& surface) const;

	public:
		VkPhysicalDevice GetHandle() const { return mPhysicalDeviceHandle; }

		VkPhysicalDeviceFeatures               GetFeatures()                  const { return mFeatures;                  }
		VkPhysicalDeviceProperties             GetProperties()                const { return mProperties;                }
		const Vector<VkExtensionProperties>&   GetDeviceExtensionProperties() const { return mDeviceExtensionProperties; }
		const Vector<VkQueueFamilyProperties>& GetQueueFamilies()             const { return mQueueFamilies;             }

		const VkQueueFamilyProperties& GetQueueFamily(int32 index) const { return mQueueFamilies[index]; }

	private:
		VkPhysicalDevice mPhysicalDeviceHandle;

		VkPhysicalDeviceFeatures        mFeatures;
		VkPhysicalDeviceProperties      mProperties;
		Vector<VkExtensionProperties>   mDeviceExtensionProperties;
		Vector<VkQueueFamilyProperties> mQueueFamilies;

		Ref<VulkanInstance> mInstance;
	};

}
