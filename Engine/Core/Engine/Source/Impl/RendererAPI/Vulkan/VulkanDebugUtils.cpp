#include "cspch.hpp"
#include "VulkanDebugUtils.hpp"

namespace Cosmic::Utils
{

	static ELogSeverity VkDebugUtilsMessageSeverityFlagBitsEXTToELogSeverity(VkDebugUtilsMessageSeverityFlagBitsEXT severity)
	{
		switch (severity)
		{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: return ELogSeverity::Trace;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:    return ELogSeverity::Info;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: return ELogSeverity::Warn;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:   return ELogSeverity::Error;
		}	

		return (ELogSeverity)-1;
	}

	static const char* VkDebugUtilsMessageTypeFlagsEXTToString(VkDebugUtilsMessageTypeFlagsEXT type)
	{
		switch (type)
		{
			case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:                return "General";
			case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:             return "Validation";
			case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:            return "Performance";
			case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT: return "Device Address Binding";
		}

		return "Unknown";
	}


	static const char* VkObjectTypeToString(VkObjectType type)
	{
		switch (type)
		{
			case VK_OBJECT_TYPE_UNKNOWN:                      return "Unknown";
			case VK_OBJECT_TYPE_INSTANCE:                     return "VkInstance";
			case VK_OBJECT_TYPE_PHYSICAL_DEVICE:              return "VkPhysicalDevice";
			case VK_OBJECT_TYPE_DEVICE:                       return "VkDevice";
			case VK_OBJECT_TYPE_QUEUE:                        return "VkQueue";
			case VK_OBJECT_TYPE_SEMAPHORE:                    return "VkSemaphore";
			case VK_OBJECT_TYPE_COMMAND_BUFFER:               return "VkCommandBuffer";
			case VK_OBJECT_TYPE_FENCE:                        return "VkFence";
			case VK_OBJECT_TYPE_DEVICE_MEMORY:                return "VkDeviceMemory";
			case VK_OBJECT_TYPE_BUFFER:                       return "VkBuffer";
			case VK_OBJECT_TYPE_IMAGE:                        return "VkImage";
			case VK_OBJECT_TYPE_EVENT:                        return "VkEvent";
			case VK_OBJECT_TYPE_QUERY_POOL:                   return "VkQueryPool";
			case VK_OBJECT_TYPE_BUFFER_VIEW:                  return "VkBufferView";
			case VK_OBJECT_TYPE_IMAGE_VIEW:                   return "VkImageView";
			case VK_OBJECT_TYPE_SHADER_MODULE:                return "VkShaderModule";
			case VK_OBJECT_TYPE_PIPELINE_CACHE:               return "VkPipelineCache";
			case VK_OBJECT_TYPE_PIPELINE_LAYOUT:              return "VkPipelineLayout";
			case VK_OBJECT_TYPE_RENDER_PASS:                  return "VkRenderPass";
			case VK_OBJECT_TYPE_PIPELINE:                     return "VkPipeline";
			case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:        return "VkDescriptorSetLayout";
			case VK_OBJECT_TYPE_SAMPLER:                      return "VkSampler";
			case VK_OBJECT_TYPE_DESCRIPTOR_POOL:              return "VkDescriptorPool";
			case VK_OBJECT_TYPE_DESCRIPTOR_SET:               return "VKDescriptorSet";
			case VK_OBJECT_TYPE_FRAMEBUFFER:                  return "VkFramebuffer";
			case VK_OBJECT_TYPE_COMMAND_POOL:                 return "VkCommandPool";
			case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION:     return "VkSamplerYcbcrConversion";
			case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE:   return "VkDescriptorUpdateTemplate";
			case VK_OBJECT_TYPE_PRIVATE_DATA_SLOT:            return "VkPrivateDataSlot";
			case VK_OBJECT_TYPE_SURFACE_KHR:                  return "VkSurfaceKHR";
			case VK_OBJECT_TYPE_SWAPCHAIN_KHR:                return "VkSwapchainKHR";
			case VK_OBJECT_TYPE_DISPLAY_KHR:                  return "VkDisplayKHR";
			case VK_OBJECT_TYPE_DISPLAY_MODE_KHR:             return "VkDisplayModeKHR";
			case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT:    return "VkDebugReportCallbackEXT";
			case VK_OBJECT_TYPE_VIDEO_SESSION_KHR:            return "VkVideoSessionKHR";
			case VK_OBJECT_TYPE_VIDEO_SESSION_PARAMETERS_KHR: return "VkVideoSessionParemetersKHR";
			case VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT:    return "VkDebugUtilsMessengerEXT";
		}

		return "Unknown";
	}

	static const char* VkPhysicalDeviceTypeToString(VkPhysicalDeviceType type)
	{
		switch (type)
		{
			case VK_PHYSICAL_DEVICE_TYPE_OTHER:          return "Other";
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "Integrated GPU";
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:   return "Discrete GPU";
			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:    return "Virtual GPU";
			case VK_PHYSICAL_DEVICE_TYPE_CPU:            return "CPU";
		}

		return "Unknown";
	}

}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
{
	using namespace Cosmic;

	ELogSeverity severity       = Utils::VkDebugUtilsMessageSeverityFlagBitsEXTToELogSeverity(messageSeverity);
	const char*  messageTypeStr = Utils::VkDebugUtilsMessageTypeFlagsEXTToString(messageType);

	Log::LogWithSeverity("Vulkan Message ({}): {}", severity, messageTypeStr, callbackData->pMessage);
	Log::LogWithSeverity("Vulkan Objects:", severity);

	for (uint32 i = 0; i < callbackData->objectCount; i++)
	{
		const VkDebugUtilsObjectNameInfoEXT& objectNameInfo = callbackData->pObjects[i];

		const char* objectTypeStr = Utils::VkObjectTypeToString(objectNameInfo.objectType);
		uint64      objectHandle  = objectNameInfo.objectHandle;

		Log::LogWithSeverity("\t- {} ({})", severity, objectTypeStr, objectHandle);
	}

	return VK_FALSE;
}

namespace Cosmic
{


}