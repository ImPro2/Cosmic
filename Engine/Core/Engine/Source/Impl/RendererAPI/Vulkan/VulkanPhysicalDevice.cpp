#include "cspch.hpp"
#include "VulkanPhysicalDevice.hpp"

#include "VulkanInstance.hpp"
#include "VulkanSurface.hpp"

namespace Cosmic
{

	VulkanPhysicalDevice::VulkanPhysicalDevice(const Ref<VulkanInstance>& instance, VkPhysicalDevice physicalDeviceHandle)
		: mInstance(instance), mPhysicalDeviceHandle(physicalDeviceHandle)
	{
		vkGetPhysicalDeviceFeatures(mPhysicalDeviceHandle, &mFeatures);
		vkGetPhysicalDeviceProperties(mPhysicalDeviceHandle, &mProperties);

		uint32 queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDeviceHandle, &queueFamilyCount, nullptr);

		mQueueFamilies.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDeviceHandle, &queueFamilyCount, mQueueFamilies.data());

		uint32 deviceExtensionPropertyCount;
		vkEnumerateDeviceExtensionProperties(mPhysicalDeviceHandle, nullptr, &deviceExtensionPropertyCount, nullptr);

		mDeviceExtensionProperties.resize(deviceExtensionPropertyCount);
		vkEnumerateDeviceExtensionProperties(mPhysicalDeviceHandle, nullptr, &deviceExtensionPropertyCount, mDeviceExtensionProperties.data());
	}

	VulkanPhysicalDevice::~VulkanPhysicalDevice()
	{
	}

	bool VulkanPhysicalDevice::SupportsDeviceExtension(const char* extensionName) const
	{
		for (const VkExtensionProperties& extensionProperties : mDeviceExtensionProperties)
		{
			if (strcmp(extensionProperties.extensionName, extensionName) == 0)
			{
				return true;
			}
		}

		return false;
	}

	bool VulkanPhysicalDevice::SupportsDeviceExtensions(const Vector<const char*>& extensionNames) const
	{
		for (const char* extensionName : extensionNames)
		{
			if (!SupportsDeviceExtension(extensionName))
			{
				return false;
			}
		}

		return true;
	}

	bool VulkanPhysicalDevice::SupportsDeviceFeatures(const VkPhysicalDeviceFeatures& features) const
	{
		if (features.robustBufferAccess                      && !mFeatures.robustBufferAccess)                      return false;
		if (features.fullDrawIndexUint32                     && !mFeatures.fullDrawIndexUint32)                     return false;
		if (features.imageCubeArray                          && !mFeatures.imageCubeArray)                          return false;
		if (features.independentBlend                        && !mFeatures.independentBlend)                        return false;
		if (features.geometryShader                          && !mFeatures.geometryShader)                          return false;
		if (features.tessellationShader                      && !mFeatures.tessellationShader)                      return false;
		if (features.sampleRateShading                       && !mFeatures.sampleRateShading)                       return false;
		if (features.dualSrcBlend                            && !mFeatures.dualSrcBlend)                            return false;
		if (features.logicOp                                 && !mFeatures.logicOp)                                 return false;
		if (features.multiDrawIndirect                       && !mFeatures.multiDrawIndirect)                       return false;
		if (features.drawIndirectFirstInstance               && !mFeatures.drawIndirectFirstInstance)               return false;
		if (features.depthClamp                              && !mFeatures.depthClamp)                              return false;
		if (features.depthBiasClamp                          && !mFeatures.depthBiasClamp)                          return false;
		if (features.fillModeNonSolid                        && !mFeatures.fillModeNonSolid)                        return false;
		if (features.depthBounds                             && !mFeatures.depthBounds)                             return false;
		if (features.wideLines                               && !mFeatures.wideLines)                               return false;
		if (features.largePoints                             && !mFeatures.largePoints)                             return false;
		if (features.alphaToOne                              && !mFeatures.alphaToOne)                              return false;
		if (features.multiViewport                           && !mFeatures.multiViewport)                           return false;
		if (features.samplerAnisotropy                       && !mFeatures.samplerAnisotropy)                       return false;
		if (features.textureCompressionETC2                  && !mFeatures.textureCompressionETC2)                  return false;
		if (features.textureCompressionASTC_LDR              && !mFeatures.textureCompressionASTC_LDR)              return false;
		if (features.textureCompressionBC                    && !mFeatures.textureCompressionBC)                    return false;
		if (features.occlusionQueryPrecise                   && !mFeatures.occlusionQueryPrecise)                   return false;
		if (features.pipelineStatisticsQuery                 && !mFeatures.pipelineStatisticsQuery)                 return false;
		if (features.vertexPipelineStoresAndAtomics          && !mFeatures.vertexPipelineStoresAndAtomics)          return false;
		if (features.fragmentStoresAndAtomics                && !mFeatures.fragmentStoresAndAtomics)                return false;
		if (features.shaderTessellationAndGeometryPointSize  && !mFeatures.shaderTessellationAndGeometryPointSize)  return false;
		if (features.shaderImageGatherExtended               && !mFeatures.shaderImageGatherExtended)               return false;
		if (features.shaderStorageImageExtendedFormats       && !mFeatures.shaderStorageImageExtendedFormats)       return false;
		if (features.shaderStorageImageMultisample           && !mFeatures.shaderStorageImageMultisample)           return false;
		if (features.shaderStorageImageReadWithoutFormat     && !mFeatures.shaderStorageImageReadWithoutFormat)     return false;
		if (features.shaderStorageImageWriteWithoutFormat    && !mFeatures.shaderStorageImageWriteWithoutFormat)    return false;
		if (features.shaderUniformBufferArrayDynamicIndexing && !mFeatures.shaderUniformBufferArrayDynamicIndexing) return false;
		if (features.shaderSampledImageArrayDynamicIndexing  && !mFeatures.shaderSampledImageArrayDynamicIndexing)  return false;
		if (features.shaderStorageBufferArrayDynamicIndexing && !mFeatures.shaderStorageBufferArrayDynamicIndexing) return false;
		if (features.shaderStorageImageArrayDynamicIndexing  && !mFeatures.shaderStorageImageArrayDynamicIndexing)  return false;
		if (features.shaderClipDistance                      && !mFeatures.shaderClipDistance)                      return false;
		if (features.shaderCullDistance                      && !mFeatures.shaderCullDistance)                      return false;
		if (features.shaderFloat64                           && !mFeatures.shaderFloat64)                           return false;
		if (features.shaderInt64                             && !mFeatures.shaderInt64)                             return false;
		if (features.shaderInt16                             && !mFeatures.shaderInt16)                             return false;
		if (features.shaderResourceResidency                 && !mFeatures.shaderResourceResidency)                 return false;
		if (features.shaderResourceMinLod                    && !mFeatures.shaderResourceMinLod)                    return false;
		if (features.sparseBinding                           && !mFeatures.sparseBinding)                           return false;
		if (features.sparseResidencyBuffer                   && !mFeatures.sparseResidencyBuffer)                   return false;
		if (features.sparseResidencyImage2D                  && !mFeatures.sparseResidencyImage2D)                  return false;
		if (features.sparseResidencyImage3D                  && !mFeatures.sparseResidencyImage3D)                  return false;
		if (features.sparseResidency2Samples                 && !mFeatures.sparseResidency2Samples)                 return false;
		if (features.sparseResidency4Samples                 && !mFeatures.sparseResidency4Samples)                 return false;
		if (features.sparseResidency8Samples                 && !mFeatures.sparseResidency8Samples)                 return false;
		if (features.sparseResidency16Samples                && !mFeatures.sparseResidency16Samples)                return false;
		if (features.sparseResidencyAliased                  && !mFeatures.sparseResidencyAliased)                  return false;
		if (features.variableMultisampleRate                 && !mFeatures.variableMultisampleRate)                 return false;
		if (features.inheritedQueries                        && !mFeatures.inheritedQueries)                        return false;

		return true;
	}

	QueueFamilyIndices VulkanPhysicalDevice::GetQueueFamilyIndices(VkQueueFlags flags, const Ref<VulkanSurface>& surface) const
	{
		QueueFamilyIndices indices;

		for (uint32 i = 0; i < mQueueFamilies.size(); i++)
		{
			const VkQueueFamilyProperties& queueFamilyProperties = mQueueFamilies[i];

			bool queueMatched = (queueFamilyProperties.queueFlags & flags) == flags;

			VkBool32 presentSupported = VK_FALSE;
			VK_CALL(vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDeviceHandle, i, surface->GetHandle(), &presentSupported));

			if (queueMatched)
				indices.GraphicsFamily = i;

			if (presentSupported == VK_TRUE)
				indices.PresentFamily = i;

			if (indices.GraphicsFamily != -1 && indices.PresentFamily != -1)
				break;
		}

		return indices;
	}

}