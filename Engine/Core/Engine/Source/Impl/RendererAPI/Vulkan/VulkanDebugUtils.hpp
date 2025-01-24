#pragma once
#include "App/Log/Log.hpp"
#include "Base/Base.hpp"

#include <vulkan/vulkan.h>

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData);

namespace Cosmic
{

	inline void VulkanError(VkResult result, const char* file, int32 line, const char* functionString)
	{
		Log::Error("Vulkan Error ({}:{}): {}", file, line, functionString);
		CS_BREAK();
	}

}

#define VK_ERROR(result, msg) ::Cosmic::VulkanError(result, __FILE__, __LINE__, msg)

#define VK_CALL(fn)                         \
do                                          \
{                                           \
	VkResult result = fn;                   \
	if (result != VK_SUCCESS)               \
		VK_ERROR(result, CS_STRINGIFY(fn)); \
} while (0)
