#pragma once
#include "VulkanGraphicsContext.hpp"

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if 0
namespace Cosmic::VKInternal
{



	void InitVulkan(bool enableValidationLayers = true);
	void ShutdownVulkan();

	VkSurfaceKHR     CreateSurface(GLFWwindow* windowHandle);
	VkPhysicalDevice CreatePhysicalDevice();
	VkDevice         CreateLogicalDevice(VkPhysicalDevice physicalDeviceHandle);
	VkImageView      CreateImageView(VkImage imageHandle, VkFormat format, VkImageAspectFlags aspectFlags, uint32 mipLevels);
	

	bool IsValidationLayersEnabled();

	Vector<const char*> GetValidationLayerNames();
	Vector<const char*> GetDeviceExtensionNames();
	Vector<const char*> GetInstanceExtensionNames();

	VkInstance                 GetInstance();
	Ref<VulkanGraphicsContext> GetGraphicsContext();

	SwapchainSupportDetails QuerySwapchainSupportDetails(VkSurfaceKHR surfaceHandle, VkPhysicalDevice physicalDevice);
}

#endif

