#pragma once

#include <vulkan/vulkan.h>

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT             messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
	void* userData
);

VKAPI_ATTR VkResult VKAPI_CALL VulkanCreateDebugUtilsMessengerEXT(
	VkInstance                                instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks*              pAllocator,
	VkDebugUtilsMessengerEXT*                 pMessenger
);

VKAPI_ATTR void VKAPI_CALL VulkanDestroyDebugUtilsMessengerEXT(
	VkInstance                   instance,
	VkDebugUtilsMessengerEXT     messenger,
	const VkAllocationCallbacks* pAllocator
);

void VulkanError(VkResult result, const char* file, Cosmic::int32 line, const char* function);

#if 0
#define VK_CALL(fn)                                            \
do                                                             \
{                                                              \
	VkResult result = fn;                                      \
	if (result != VK_SUCCESS)                                  \
		VulkanError(result, __FILE__, __LINE__, __FUNCTION__); \
} while (0)
#else
#define VK_CALL(fn)
#endif
