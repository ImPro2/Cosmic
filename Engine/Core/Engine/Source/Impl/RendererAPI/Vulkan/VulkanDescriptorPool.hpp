#pragma once
#include "VulkanDebugUtils.hpp"
#include "VulkanDescriptorSet.hpp"

#include "Renderer/ShaderReflectionData.hpp"

namespace Cosmic
{

	class VulkanDevice;

	class VulkanDescriptorPool : public IRefCounted
	{
	public:
		VulkanDescriptorPool(const Ref<VulkanDevice>& device, uint32 maxDescriptorSetCount, const UnorderedMap<EShaderResourceType, uint32> descriptorTypeCountMap, VkDescriptorPoolCreateFlags flags = 0);
		~VulkanDescriptorPool();

	public:
		Vector<Ref<VulkanDescriptorSet>> AllocateDescriptorSets(const Vector<VkDescriptorSetLayout>& descriptorSetLayoutHandles);

	public:
		VkDescriptorPool GetHandle() const { return mHandle; }

	private:
		VkDescriptorPool mHandle;

		uint32 mMaxDescriptorSetCount = 0;
		UnorderedMap<EShaderResourceType, uint32> mDescriptorTypeCountMap;

		Ref<VulkanDevice> mDevice;
	};

}

