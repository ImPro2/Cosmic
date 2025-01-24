#include "cspch.hpp"
#include "VulkanDevice.hpp"

CS_MODULE_LOG_INFO(Cosmic, Impl.RendererAPI.Vulkan.VulkanDevice);

namespace Cosmic
{

	Vector<VkExtensionProperties> VulkanGetSupportedDeviceExtensionProperties(VkPhysicalDevice physicalDeviceHandle, const char* layerName)
	{
		uint32 count;
		vkEnumerateDeviceExtensionProperties(physicalDeviceHandle, layerName, &count, nullptr);

		Vector<VkExtensionProperties> extensions(count);
		vkEnumerateDeviceExtensionProperties(physicalDeviceHandle, layerName, &count, extensions.data());

		return extensions;
	}

	VulkanDevice::VulkanDevice(const Vector<VulkanQueueCreateInfo>& queues, const Ref<VulkanPhysicalDevice>& physicalDevice, const VkPhysicalDeviceFeatures& physicalDeviceFeatures, const Vector<const char*>& requiredDeviceExtensionNames)
		: mPhysicalDevice(physicalDevice)
	{
		mDeviceExtensions = FilterSupportedDeviceExtensions(requiredDeviceExtensionNames);
		mEnabledFeatures  = physicalDeviceFeatures;

		Vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos = GetVkDeviceQueueCreateInfos(queues);

		CreateDeviceHandle(deviceQueueCreateInfos);
		CreateQueues(deviceQueueCreateInfos);
	}

	VulkanDevice::~VulkanDevice()
	{
		mQueues.clear();

		vkDestroyDevice(mDeviceHandle, nullptr);
	}

	Vector<VkExtensionProperties> VulkanDevice::FilterSupportedDeviceExtensions(const Vector<const char*>& deviceExtensionNames)
	{
		Vector<VkExtensionProperties> supportedExtensions = VulkanGetSupportedDeviceExtensionProperties(mPhysicalDevice->GetHandle());
		Vector<VkExtensionProperties> filteredExtensions;
		filteredExtensions.reserve(deviceExtensionNames.size());

		for (Vector<const char*>::const_iterator it = deviceExtensionNames.begin(); it != deviceExtensionNames.end(); it++)
		{
			Vector<VkExtensionProperties>::const_iterator supportedExtensionsIt = std::find_if(supportedExtensions.begin(), supportedExtensions.end(), [it](const VkExtensionProperties& extensionProperties) -> bool
			{
				return strcmp(extensionProperties.extensionName, *it) == 0;
			});

			if (supportedExtensionsIt != supportedExtensions.end())
			{
				filteredExtensions.push_back(*supportedExtensionsIt);
				CS_LOG_DEBUG("Enabled Vulkan Device Extension: {}", *it);
			}
			else
			{
				CS_LOG_WARN("Attempted to enable unsupported Vulkan Device Extension: {}", *it);
			}
		}

		return filteredExtensions;
	}

	Vector<VkDeviceQueueCreateInfo> VulkanDevice::GetVkDeviceQueueCreateInfos(const Vector<VulkanQueueCreateInfo>& queues)
	{
		Vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;

		for (uint32 i = 0; i < queues.size(); i++)
		{
			bool unique = true;

			for (uint32 j = 0; j < i; j++)
			{
				if (i == j)
					continue;

				if (queues[i].QueueFamilyIndex == queues[j].QueueFamilyIndex)
				{
					unique = false;
					break;
				}
			}

			if (!unique)
				continue;

			VkDeviceQueueCreateInfo deviceQueueCreateInfo = {
				.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.queueFamilyIndex = (uint32)queues[i].QueueFamilyIndex,
				.queueCount       = (uint32)queues[i].QueuePriorities.size(),
				.pQueuePriorities = queues[i].QueuePriorities.data()
			};

			deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
		}

		return deviceQueueCreateInfos;
	}

	void VulkanDevice::CreateDeviceHandle(const Vector<VkDeviceQueueCreateInfo>& deviceQueueCreateInfos)
	{
		Vector<const char*> extensionNames(mDeviceExtensions.size());

		for (uint32 i = 0; i < mDeviceExtensions.size(); i++)
			extensionNames[i] = mDeviceExtensions[i].extensionName;

		VkDeviceCreateInfo deviceCreateInfo = {
			.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.queueCreateInfoCount    = (uint32)deviceQueueCreateInfos.size(),
			.pQueueCreateInfos       = deviceQueueCreateInfos.data(),
			.enabledExtensionCount   = (uint32)extensionNames.size(),
			.ppEnabledExtensionNames = extensionNames.data(),
			.pEnabledFeatures        = &mEnabledFeatures
		};

		VK_CALL(vkCreateDevice(mPhysicalDevice->GetHandle(), &deviceCreateInfo, nullptr, &mDeviceHandle));
	}

	void VulkanDevice::CreateQueues(const Vector<VkDeviceQueueCreateInfo>& deviceQueueCreateInfos)
	{
		for (const VkDeviceQueueCreateInfo& deviceQueueCreateInfo : deviceQueueCreateInfos)
		{
			for (uint32 queueIndex = 0; queueIndex < deviceQueueCreateInfo.queueCount; queueIndex++)
			{
				VkQueueFlags queueFlags       = mPhysicalDevice->GetQueueFamily(deviceQueueCreateInfo.queueFamilyIndex).queueFlags;
				int32        queueFamilyIndex = deviceQueueCreateInfo.queueFamilyIndex;

				VkQueue queueHandle;
				vkGetDeviceQueue(mDeviceHandle, deviceQueueCreateInfo.queueFamilyIndex, queueIndex, &queueHandle);

				mQueues.push_back(CreateRef<VulkanQueue>(this, queueHandle, queueFlags, queueFamilyIndex, queueIndex));
			}
		}
	}

	Ref<VulkanQueue> VulkanDevice::GetQueue(int32 queueFamilyIndex, uint32 queueIndex)
	{
		for (Ref<VulkanQueue> queue : mQueues)
		{
			if (queue->GetQueueFamilyIndex() == queueFamilyIndex && queue->GetQueueIndex() == queueFamilyIndex)
			{
				return queue;
			}
		}

		return nullptr;
	}

}