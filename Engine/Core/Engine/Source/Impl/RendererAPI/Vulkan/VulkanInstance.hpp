#pragma once
#include "VulkanDebugUtils.hpp"
#include "VulkanStructs.hpp"

namespace Cosmic
{

	class VulkanInstance;
	class VulkanSurface;
	class VulkanPhysicalDevice;

	struct VulkanInstanceDebugUtilsExtensionCallbacks
	{
		PFN_vkCmdBeginDebugUtilsLabelEXT    vkCmdBeginDebugUtilsLabelEXT    = nullptr;
		PFN_vkCmdEndDebugUtilsLabelEXT      vkCmdEndDebugUtilsLabelEXT      = nullptr;
		PFN_vkCmdInsertDebugUtilsLabelEXT   vkCmdInsertDebugUtilsLabelEXT   = nullptr;
		PFN_vkCreateDebugUtilsMessengerEXT  vkCreateDebugUtilsMessengerEXT  = nullptr;
		PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = nullptr;
		PFN_vkQueueBeginDebugUtilsLabelEXT  vkQueueBeginDebugUtilsLabelEXT  = nullptr;
		PFN_vkQueueEndDebugUtilsLabelEXT    vkQueueEndDebugUtilsLabelEXT    = nullptr;
		PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT = nullptr;
		PFN_vkSetDebugUtilsObjectNameEXT    vkSetDebugUtilsObjectNameEXT    = nullptr;
		PFN_vkSetDebugUtilsObjectTagEXT     vkSetDebugUtilsObjectTagEXT     = nullptr;
		PFN_vkSubmitDebugUtilsMessageEXT    vkSubmitDebugUtilsMessageEXT    = nullptr;
	};

	struct VulkanInstanceExtensionCallbacks
	{
		VulkanInstanceDebugUtilsExtensionCallbacks DebugUtilsExtensionCallbacks;
	};

	Vector<const char*> VulkanGetRequiredInstanceExtensionNames();

	Vector<VkExtensionProperties> VulkanGetSupportedInstanceExtensionProperties(const char* layerName = nullptr);
	Vector<VkLayerProperties>     VulkanGetSupportedInstanceLayerProperties();
	Vector<VkPhysicalDevice>      VulkanGetPhysicalDevices(VkInstance instanceHandle);

	struct VulkanPhysicalDevicePreferences
	{
		VkQueueFlags                  QueueFlags;
		Vector<VkPhysicalDeviceType>  PreferredDeviceTypes;
		Vector<const char*>           RequiredDeviceExtensionNames;
		VkPhysicalDeviceFeatures      RequiredDeviceFeatures;
	};

	class VulkanInstance : public IRefCounted
	{
	public:
		VulkanInstance(const Vector<const char*>& requiredExtensionNames, const Vector<const char*>& requiredLayerNames, uint32 vulkanAPIVersion = VK_API_VERSION_1_3);
		~VulkanInstance();

	public:
		template<typename F>
		bool GetProcAddr(F& procAddress, const char* name)
		{
			procAddress = (F)vkGetInstanceProcAddr(mInstanceHandle, name);
			return procAddress;
		}

	public:
		VkInstance               GetHandle()                    const { return mInstanceHandle;            }
		VkDebugUtilsMessengerEXT GetDebugUtilsMessengerHandle() const { return mDebugUtilsMessengerHandle; }

		const VulkanInstanceExtensionCallbacks& GetExtensionCallbacks() const { return mExtensionCallbacks; }

		uint32 GetVulkanAPIVersion() const { return mVulkanAPIVersion; }

		const Vector<VkExtensionProperties>& GetExtensions() const { return mExtensions; }
		const Vector<VkLayerProperties>&     GetLayers()     const { return mLayers;     }

		bool IsValidationLayersEnabled() const { return mValidationLayersEnabled; }

		const Vector<Ref<VulkanPhysicalDevice>>& GetPhysicalDevices() const { return mPhysicalDevices; }

	public:
		Pair<Ref<VulkanPhysicalDevice>, QueueFamilyIndices> ChoosePhysicalDeviceAndQueueFamilyIndices(const Ref<VulkanSurface>& surface, const VulkanPhysicalDevicePreferences& preferences);

	private:
		void CreateInstanceHandle();
		void CreateDebugUtilsMessengerHandle();
		void CreatePhysicalDevices();

		Vector<VkExtensionProperties> FilterSupportedInstanceExtensions(const Vector<const char*>& extensionNames);
		Vector<VkLayerProperties>     FilterSupportedInstanceLayers(const Vector<const char*>& layerNames);

		void LoadInstanceExtensions();

	private:
		VkInstance                         mInstanceHandle            = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT           mDebugUtilsMessengerHandle = VK_NULL_HANDLE;
		VkDebugUtilsMessengerCreateInfoEXT mDebugUtilsMessengerCreateInfo;

		VulkanInstanceExtensionCallbacks mExtensionCallbacks;

		uint32                        mVulkanAPIVersion;
		Vector<VkExtensionProperties> mExtensions;
		Vector<VkLayerProperties>     mLayers;

		bool mValidationLayersEnabled = false;

		Vector<Ref<VulkanPhysicalDevice>> mPhysicalDevices;
	};

}
