#include "cspch.hpp"
#include "VulkanInstance.hpp"
#include "VulkanPhysicalDevice.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

CS_MODULE_LOG_INFO(Cosmic, Impl.RendererAPI.Vulkan.VulkanInstance);

namespace Cosmic
{
	
	Vector<const char*> VulkanGetRequiredInstanceExtensionNames()
	{
		uint32 count;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);

		return Vector<const char*>(glfwExtensions, glfwExtensions + count);
	}

	Vector<VkExtensionProperties> VulkanGetSupportedInstanceExtensionProperties(const char* layerName)
	{
		Vector<VkExtensionProperties> supportedExtensionProperties;

		uint32 count;
		vkEnumerateInstanceExtensionProperties(layerName, &count, nullptr);

		supportedExtensionProperties.resize(count);

		vkEnumerateInstanceExtensionProperties(layerName, &count, supportedExtensionProperties.data());

		return supportedExtensionProperties;
	}

	Vector<VkLayerProperties> VulkanGetSupportedInstanceLayerProperties()
	{
		Vector<VkLayerProperties> supportedLayerProperties;

		uint32 count;
		vkEnumerateInstanceLayerProperties(&count, nullptr);

		supportedLayerProperties.resize(count);

		vkEnumerateInstanceLayerProperties(&count, supportedLayerProperties.data());

		return supportedLayerProperties;
	}

	Vector<VkPhysicalDevice> VulkanGetPhysicalDevices(VkInstance instanceHandle)
	{
		uint32 count;
		vkEnumeratePhysicalDevices(instanceHandle, &count, nullptr);

		Vector<VkPhysicalDevice> physicalDevices(count);
		vkEnumeratePhysicalDevices(instanceHandle, &count, physicalDevices.data());

		return physicalDevices;
	}

	VulkanInstance::VulkanInstance(const Vector<const char*>& requiredExtensionNames, const Vector<const char*>& requiredLayerNames, uint32 vulkanAPIVersion)
		: mVulkanAPIVersion(vulkanAPIVersion)
	{
		mExtensions = FilterSupportedInstanceExtensions(requiredExtensionNames);
		mLayers     = FilterSupportedInstanceLayers(requiredLayerNames);

		{
			Vector<VkLayerProperties>::const_iterator validationLayerIt = std::find_if(mLayers.begin(), mLayers.end(), [](const VkLayerProperties& properties) -> bool
			{
				return strcmp(properties.layerName, "VK_LAYER_KHRONOS_validation") == 0;
			});

			mValidationLayersEnabled = validationLayerIt != mLayers.end();

			if (mValidationLayersEnabled)
			{
				mDebugUtilsMessengerCreateInfo = {
					.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
					.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
					.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
					.pfnUserCallback = VulkanDebugCallback
				};
			}
		}

		CreateInstanceHandle();
		LoadInstanceExtensions();

		if (mValidationLayersEnabled)
			CreateDebugUtilsMessengerHandle();

		CreatePhysicalDevices();
	}

	VulkanInstance::~VulkanInstance()
	{
		mPhysicalDevices.clear();

		if (mDebugUtilsMessengerHandle != VK_NULL_HANDLE && mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkDestroyDebugUtilsMessengerEXT)
			mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkDestroyDebugUtilsMessengerEXT(mInstanceHandle, mDebugUtilsMessengerHandle, nullptr);

		vkDestroyInstance(mInstanceHandle, nullptr);
	}

	void VulkanInstance::CreateInstanceHandle()
	{
		VkApplicationInfo applicationInfo = {
			.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName   = "Cosmic Application",
			.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
			.pEngineName        = "Cosmic Engine",
			.engineVersion      = VK_MAKE_VERSION(1, 0, 0),
			.apiVersion         = VK_API_VERSION_1_0
		};

		Vector<const char*> layerNames(mLayers.size());
		Vector<const char*> extensionNames(mExtensions.size());

		for (uint32 i = 0; i < layerNames.size(); i++)
			layerNames[i] = mLayers[i].layerName;

		for (uint32 i = 0; i < mExtensions.size(); i++)
			extensionNames[i] = mExtensions[i].extensionName;

		VkInstanceCreateInfo instanceCreateInfo = {
			.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.flags                   = 0,
			.pApplicationInfo        = &applicationInfo,
			.enabledLayerCount       = (uint32)layerNames.size(),
			.ppEnabledLayerNames     = layerNames.data(),
			.enabledExtensionCount   = (uint32)extensionNames.size(),
			.ppEnabledExtensionNames = extensionNames.data(),
		};

		if (mValidationLayersEnabled)
		{
			instanceCreateInfo.pNext = &mDebugUtilsMessengerCreateInfo;
		}

		VK_CALL(vkCreateInstance(&instanceCreateInfo, nullptr, &mInstanceHandle));
	}

	void VulkanInstance::CreateDebugUtilsMessengerHandle()
	{
		VK_CALL(mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkCreateDebugUtilsMessengerEXT(mInstanceHandle, &mDebugUtilsMessengerCreateInfo, nullptr, &mDebugUtilsMessengerHandle));
	}

	void VulkanInstance::CreatePhysicalDevices()
	{
		for (VkPhysicalDevice physicalDeviceHandle : VulkanGetPhysicalDevices(mInstanceHandle))
		{
			mPhysicalDevices.push_back(CreateRef<VulkanPhysicalDevice>(this, physicalDeviceHandle));
		}
	}

	Vector<VkExtensionProperties> VulkanInstance::FilterSupportedInstanceExtensions(const Vector<const char*>& extensionNames)
	{
		Vector<VkExtensionProperties> supportedExtensions = VulkanGetSupportedInstanceExtensionProperties();
		Vector<VkExtensionProperties> filteredExtensions;
		filteredExtensions.reserve(extensionNames.size());

		for (Vector<const char*>::const_iterator it = extensionNames.begin(); it != extensionNames.end(); it++)
		{
			Vector<VkExtensionProperties>::iterator supportedExtensionsIt = std::find_if(supportedExtensions.begin(), supportedExtensions.end(), [it](const VkExtensionProperties& extensionProperties) -> bool
			{
				return strcmp(extensionProperties.extensionName, *it) == 0;
			});

			if (supportedExtensionsIt != supportedExtensions.end())
			{
				filteredExtensions.push_back(*supportedExtensionsIt);
				CS_LOG_DEBUG("Enabled Vulkan Extension: {}", *it);
			}
			else
			{
				CS_LOG_WARN("Tried enabling unsupported Vulkan Extension: {}", *it);
			}
		}

		return filteredExtensions;
	}

	Vector<VkLayerProperties> VulkanInstance::FilterSupportedInstanceLayers(const Vector<const char*>& layerNames)
	{
		Vector<VkLayerProperties> supportedLayers = VulkanGetSupportedInstanceLayerProperties();
		Vector<VkLayerProperties> filteredLayers;
		filteredLayers.reserve(layerNames.size());

		for (Vector<const char*>::const_iterator it = layerNames.begin(); it != layerNames.end(); it++)
		{
			Vector<VkLayerProperties>::const_iterator supportedLayersIt = std::find_if(supportedLayers.begin(), supportedLayers.end(), [it](const VkLayerProperties& layerProperties) -> bool
			{
				return strcmp(layerProperties.layerName, *it) == 0;
			});

			if (supportedLayersIt != supportedLayers.end())
			{
				filteredLayers.push_back(*supportedLayersIt);
				CS_LOG_DEBUG("Enabled Vulkan Layer: {}", *it);
			}
			else
			{
				CS_LOG_WARN("Attempted to enable unsupported Vulkan Layer: {}", *it);
			}
		}

		return filteredLayers;
	}

	void VulkanInstance::LoadInstanceExtensions()
	{
		for (const VkExtensionProperties& extension : mExtensions)
		{
			if (strcmp(extension.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
			{
				GetProcAddr(mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkCmdBeginDebugUtilsLabelEXT,    "vkCmdBeginDebugUtilsLabelEXT");
				GetProcAddr(mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkCmdBeginDebugUtilsLabelEXT,    "vkCmdBeginDebugUtilsLabelEXT");
				GetProcAddr(mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkCmdEndDebugUtilsLabelEXT,      "vkCmdEndDebugUtilsLabelEXT");
				GetProcAddr(mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkCmdInsertDebugUtilsLabelEXT,   "vkCmdInsertDebugUtilsLabelEXT");
				GetProcAddr(mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkCreateDebugUtilsMessengerEXT,  "vkCreateDebugUtilsMessengerEXT");
				GetProcAddr(mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkDestroyDebugUtilsMessengerEXT, "vkDestroyDebugUtilsMessengerEXT");
				GetProcAddr(mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkQueueBeginDebugUtilsLabelEXT,  "vkQueueBeginDebugUtilsLabelEXT");
				GetProcAddr(mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkQueueEndDebugUtilsLabelEXT,    "vkQueueEndDebugUtilsLabelEXT");
				GetProcAddr(mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkQueueInsertDebugUtilsLabelEXT, "vkQueueInsertDebugUtilsLabelEXT");
				GetProcAddr(mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkSetDebugUtilsObjectNameEXT,    "vkSetDebugUtilsObjectNameEXT");
				GetProcAddr(mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkSetDebugUtilsObjectTagEXT,     "vkSetDebugUtilsObjectTagEXT");
				GetProcAddr(mExtensionCallbacks.DebugUtilsExtensionCallbacks.vkSubmitDebugUtilsMessageEXT,    "vkSubmitDebugUtilsMessageEXT");
			}
		}
	}

	Pair<Ref<VulkanPhysicalDevice>, QueueFamilyIndices> VulkanInstance::ChoosePhysicalDeviceAndQueueFamilyIndices(const Ref<VulkanSurface>& surface, const VulkanPhysicalDevicePreferences& preferences)
	{
		for (Ref<VulkanPhysicalDevice> physicalDevice : mPhysicalDevices)
		{
			QueueFamilyIndices indices = physicalDevice->GetQueueFamilyIndices(preferences.QueueFlags, surface);

			int32 physicalDeviceTypeIndex = -1;

			for (int32 i = 0; i < preferences.PreferredDeviceTypes.size(); i++)
			{
				if (physicalDevice->GetProperties().deviceType == preferences.PreferredDeviceTypes[i])
				{
					physicalDeviceTypeIndex = i;
					break;
				}
			}

			bool deviceQueueFamiliesSupported = indices.GraphicsFamily != -1 && indices.PresentFamily != -1;
			bool deviceTypeSupported          = physicalDeviceTypeIndex != -1;
			bool deviceExtensionsSupported    = physicalDevice->SupportsDeviceExtensions(preferences.RequiredDeviceExtensionNames);
			bool deviceFeaturesSupported      = physicalDevice->SupportsDeviceFeatures(preferences.RequiredDeviceFeatures);

			if (deviceQueueFamiliesSupported && deviceTypeSupported && deviceExtensionsSupported && deviceFeaturesSupported)
			{
				return { physicalDevice, indices };
			}
		}

		CS_ASSERT(false, "Error: Could not meet physical device preferences.");
		return { nullptr , -1 };
	}

}