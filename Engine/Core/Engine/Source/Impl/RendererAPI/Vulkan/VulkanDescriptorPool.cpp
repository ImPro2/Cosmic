#include "cspch.hpp"
#include "VulkanDescriptorPool.hpp"
#include "VulkanDevice.hpp"
#include "VulkanShader.hpp"

namespace Cosmic
{

	VulkanDescriptorPool::VulkanDescriptorPool(const Ref<VulkanDevice>& device, uint32 maxDescriptorSetCount,  const UnorderedMap<EShaderResourceType, uint32> descriptorTypeCountMap, VkDescriptorPoolCreateFlags flags)
		: mDevice(device), mMaxDescriptorSetCount(maxDescriptorSetCount), mDescriptorTypeCountMap(descriptorTypeCountMap)
	{
		Vector<VkDescriptorPoolSize> descriptorPoolSizes;
		descriptorPoolSizes.reserve(mDescriptorTypeCountMap.size());

		for (auto [descriptorType, descriptorCount] : mDescriptorTypeCountMap)
		{
			VkDescriptorPoolSize descriptorPoolSize = {
				.type            = EShaderResourceTypeToVkDescriptorType(descriptorType),
				.descriptorCount = descriptorCount * mMaxDescriptorSetCount
			};

			descriptorPoolSizes.push_back(descriptorPoolSize);
		}

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
			.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.flags         = flags,
			.maxSets       = mMaxDescriptorSetCount,
			.poolSizeCount = (uint32)descriptorPoolSizes.size(),
			.pPoolSizes    = descriptorPoolSizes.data()
		};

		VK_CALL(vkCreateDescriptorPool(mDevice->GetHandle(), &descriptorPoolCreateInfo, nullptr, &mHandle));
	}

	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		vkDestroyDescriptorPool(mDevice->GetHandle(), mHandle, nullptr);
	}

	Vector<Ref<VulkanDescriptorSet>> VulkanDescriptorPool::AllocateDescriptorSets(const Vector<VkDescriptorSetLayout>& descriptorSetLayoutHandles)
	{
		Vector<Ref<VulkanDescriptorSet>> descriptorSets(descriptorSetLayoutHandles.size());

		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
			.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.descriptorPool     = mHandle,
			.descriptorSetCount = (uint32)descriptorSetLayoutHandles.size(),
			.pSetLayouts        = descriptorSetLayoutHandles.data(),
		};

		VkDescriptorSet* descriptorSetHandles = (VkDescriptorSet*)alloca(descriptorSets.size() * sizeof(VkDescriptorSet));

		VK_CALL(vkAllocateDescriptorSets(mDevice->GetHandle(), &descriptorSetAllocateInfo, descriptorSetHandles));

		for (uint32 i = 0; i < descriptorSets.size(); i++)
		{
			descriptorSets[i] = CreateRef<VulkanDescriptorSet>(mDevice, this, descriptorSetHandles[i]);
		}

		return descriptorSets;
	}

}