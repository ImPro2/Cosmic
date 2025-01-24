#include "cspch.hpp"
#include "VulkanInternal.hpp"
#include "VulkanDebugUtils.hpp"
#include "VulkanGraphicsContext.hpp"
#include "VulkanInstance.hpp"
#include "VulkanSurface.hpp"
#include "VulkanPhysicalDevice.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapchain.hpp"

#include "App/Application.hpp"

CS_MODULE_LOG_INFO(Cosmic, Impl.RendererAPI.Vulkan.VulkanInternal);
#if 0
namespace Cosmic::VKInternal
{

	static VkInstance               sInstanceHandle;
	static VkDebugUtilsMessengerEXT sDebugUtilsMessengerHandle;
	static VkPhysicalDevice         sPhysicalDeviceHandle;
	static VkDevice                 sLogicalDeviceHandle;

	static bool sVulkanInitialized = false;
	static bool sEnableValidationLayers;

	static Vector<const char*> sDeviceExtensionNames = {
		"VK_LAYER_KHRONOS_validation",
	};

	static Vector<const char*> sValidationLayerNames = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	};

	static Vector<const char*> sInstanceExtensionNames;

	static void CreateInstance();
	static void CreateDebugUtilsMessenger();

	static bool VerifyValidationLayersSupport();
	static void QueryAvailableInstanceExtensions();

	static VkDebugUtilsMessengerCreateInfoEXT ConstructDebugUtilsMessengerCreateInfoEXT();

	void InitVulkan(bool enableValidationLayers)
	{
		if (sVulkanInitialized)
			return;

		sVulkanInitialized      = true;
		sEnableValidationLayers = enableValidationLayers;

		CreateInstance();
		CreateDebugUtilsMessenger();
	}

	void ShutdownVulkan()
	{
		vkDestroyDebugUtilsMessengerEXT(sInstanceHandle, sDebugUtilsMessengerHandle, nullptr);
		vkDestroyInstance(sInstanceHandle, nullptr);
	}

	VkSurfaceKHR CreateSurface(GLFWwindow* windowHandle)
	{
#ifdef CS_PLATFORM_WINDOWS

#endif

		return {};
	}

	VkPhysicalDevice CreatePhysicalDevice()
	{
		uint32 deviceCount = 0;
		VK_CALL(vkEnumeratePhysicalDevices(sInstanceHandle, &deviceCount, nullptr));

		CS_ASSERT(deviceCount != 0, "No GPU Physical Device Found");

		VkPhysicalDevice* physicalDevices = (VkPhysicalDevice*)alloca(deviceCount * sizeof(VkPhysicalDevice));
		VK_CALL(vkEnumeratePhysicalDevices(sInstanceHandle, &deviceCount, physicalDevices));

		int32 suitablePhysicalDeviceIndex = -1;

		for (uint32 i = 0; i < deviceCount; i++)
		{

		}

		// Probably not worth crashing over
		if (suitablePhysicalDeviceIndex == -1)
		{
			CS_LOG_WARN("No suitable GPU found");
			suitablePhysicalDeviceIndex = 0;
		}

		return physicalDevices[suitablePhysicalDeviceIndex];
	}

	VkDevice CreateLogicalDevice(VkPhysicalDevice physicalDeviceHandle)
	{

		VkDeviceCreateInfo deviceCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pQueueCreateInfos = nullptr,
		};
	}

	VkImageView CreateImageView(VkImage imageHandle, VkFormat format, VkImageAspectFlags aspectFlags, uint32 mipLevels)
	{


		return imageView;
	}

	bool IsValidationLayersEnabled()
	{
		return sEnableValidationLayers;
	}

	Vector<const char*> GetValidationLayerNames()
	{
		return sValidationLayerNames;
	}

	Vector<const char*> GetDeviceExtensionNames()
	{
		return sDeviceExtensionNames;
	}

	Vector<const char*> GetInstanceExtensionNames()
	{
		return sInstanceExtensionNames;
	}

	VkInstance GetInstance()
	{
		return sInstanceHandle;
	}

	Ref<VulkanGraphicsContext> GetGraphicsContext()
	{
		return Application::Get()->GetWindow()->GetGraphicsContext().As<VulkanGraphicsContext>();
	}

	SwapchainSupportDetails QuerySwapchainSupportDetails(VkSurfaceKHR surfaceHandle, VkPhysicalDevice physicalDevice)
	{
	}

	void CreateInstance()
	{
#if 0
		if (sEnableValidationLayers)
			CS_ASSERT(VerifyValidationLayersSupport(), "Required vulkan validation layers are unavailable.");

		QueryAvailableInstanceExtensions();

		VkApplicationInfo applicationInfo = {
			.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName   = "Cosmic Application",
			.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
			.pEngineName        = "Cosmic Engine",
			.engineVersion      = VK_MAKE_VERSION(1, 0, 0),
			.apiVersion         = VK_API_VERSION_1_0
		};

		VkInstanceCreateInfo instanceCreateInfo = {
			.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.flags                   = 0,
			.pApplicationInfo        = &applicationInfo,
			.enabledLayerCount       = sValidationLayerNames.size(),
			.ppEnabledLayerNames     = sValidationLayerNames.data(),
			.enabledExtensionCount   = sInstanceExtensionNames.size(),
			.ppEnabledExtensionNames = sInstanceExtensionNames.data(),
		};

		if (sEnableValidationLayers)
		{
			VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = ConstructDebugUtilsMessengerCreateInfoEXT();
			instanceCreateInfo.pNext = &debugUtilsMessengerCreateInfo;
		}

		VK_CALL(vkCreateInstance(&instanceCreateInfo, nullptr, &sInstanceHandle));
#endif
	}

	void CreateDebugUtilsMessenger()
	{
		if (!sEnableValidationLayers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = ConstructDebugUtilsMessengerCreateInfoEXT();

		//VK_CALL(Utils::VulkanCreateDebugUtilsMessengerEXT(sInstanceHandle, &debugUtilsMessengerCreateInfo, nullptr, &sDebugUtilsMessengerHandle));
	}

	void EnumeratePhysicalDevices()
	{
		uint32 deviceCount = 0;
		VK_CALL(vkEnumeratePhysicalDevices(sInstanceHandle, &deviceCount, nullptr));

		CS_ASSERT(deviceCount != 0, "No GPU Found");

		VkPhysicalDevice* physicalDevices = (VkPhysicalDevice*)alloca(deviceCount * sizeof(VkPhysicalDevice));
		VK_CALL(vkEnumeratePhysicalDevices(sInstanceHandle, &deviceCount, physicalDevices));

		for (uint32 i = 0; i < deviceCount; i++)
		{

		}
	}

	bool VerifyValidationLayersSupport()
	{
		uint32 layerCount;
		VK_CALL(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

		VkLayerProperties* availableLayers = (VkLayerProperties*)alloca(layerCount * sizeof(VkLayerProperties));

		VK_CALL(vkEnumerateInstanceLayerProperties(&layerCount, availableLayers));

		bool valid = true;

		for (const char* requiredValidationLayer : sValidationLayerNames)
		{
			bool layerFound = false;

			for (uint32 i = 0; i < layerCount; i++)
			{
				const VkLayerProperties& layerProperties = availableLayers[i];

				if (strcmp(requiredValidationLayer, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				CS_LOG_ERROR("Required Vulkan Validation Layer {} not found.", requiredValidationLayer);
				valid = false;
			}
		}

		return valid;
	}

	void QueryAvailableInstanceExtensions()
	{
		uint32 count;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);

		sInstanceExtensionNames = Vector<const char*>(glfwExtensions, glfwExtensions + count);

		if (sEnableValidationLayers)
			sInstanceExtensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	VkDebugUtilsMessengerCreateInfoEXT ConstructDebugUtilsMessengerCreateInfoEXT()
	{
		return {
			.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
			.pfnUserCallback = VulkanDebugCallback
		};
	}

	void VulkanError(VkResult result, const char* file, int32 line, const char* functionString)
	{

	}
}
#endif
