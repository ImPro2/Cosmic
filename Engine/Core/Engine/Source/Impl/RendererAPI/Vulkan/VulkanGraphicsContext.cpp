#include "cspch.hpp"
#include "VulkanGraphicsContext.hpp"

namespace Cosmic
{

	VulkanGraphicsContext::VulkanGraphicsContext(GLFWwindow* windowHandle)
		: mWindowHandle(windowHandle)
	{
	}

	VulkanGraphicsContext::~VulkanGraphicsContext()
	{
	}

	void VulkanGraphicsContext::Init()
	{
		// Create instance

		static bool instanceCreated = false;

		if (!instanceCreated)
			instanceCreated = true;
		else
			return;

#ifdef CS_DEBUG
		bool enableValidationLayers = true;
#else
		bool enableValidationLayers = false;
#endif

		Vector<const char*> instanceExtensionNames = VulkanGetRequiredInstanceExtensionNames();
		Vector<const char*> instanceLayerNames;

		if (enableValidationLayers)
		{
			instanceExtensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			instanceLayerNames.push_back("VK_LAYER_KHRONOS_validation");
		}

		mInstance = CreateRef<VulkanInstance>(instanceExtensionNames, instanceLayerNames);

		// Create surface

		mSurface = CreateRef<VulkanSurface>(mInstance, mWindowHandle);

		// Choose physical device & queue family indices

		VulkanPhysicalDevicePreferences physicalDevicePreferences = {
			.QueueFlags                   = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT,
			.PreferredDeviceTypes         = { VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU, VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU },
			.RequiredDeviceExtensionNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME },
			.RequiredDeviceFeatures       = {
				.samplerAnisotropy = VK_TRUE
			}
		};

		auto [physicalDevice, queueFamilyIndices] = mInstance->ChoosePhysicalDeviceAndQueueFamilyIndices(mSurface, physicalDevicePreferences);

		mPhysicalDevice = physicalDevice;

		// Device & queue creation

		Vector<VulkanQueueCreateInfo> queueCreateInfos;

		if (queueFamilyIndices.GraphicsFamily == queueFamilyIndices.PresentFamily)
		{
			queueCreateInfos.push_back({ queueFamilyIndices.GraphicsFamily, { 1.0f } });
		}
		else
		{
			queueCreateInfos.push_back({ queueFamilyIndices.GraphicsFamily, { 1.0f} });
			queueCreateInfos.push_back({ queueFamilyIndices.PresentFamily,  { 1.0f} });
		}

		mDevice = CreateRef<VulkanDevice>(queueCreateInfos, mPhysicalDevice, physicalDevicePreferences.RequiredDeviceFeatures, physicalDevicePreferences.RequiredDeviceExtensionNames);

		mGraphicsQueue = mDevice->GetQueue(queueFamilyIndices.GraphicsFamily);
		mPresentQueue  = mDevice->GetQueue(queueFamilyIndices.PresentFamily);

		mSwapchain = CreateRef<VulkanSwapchain>(mWindowHandle, mDevice, mSurface, mPhysicalDevice);
	}

	void VulkanGraphicsContext::Shutdown()
	{
		mSwapchain.Release();
		mDevice.Release();
		mPhysicalDevice.Release();
		mSurface.Release();
		mInstance.Release();
	}

	void VulkanGraphicsContext::Present()
	{
		//mPresentQueue->Present(mSwapchain, { mPresentSemaphore });
	}

}