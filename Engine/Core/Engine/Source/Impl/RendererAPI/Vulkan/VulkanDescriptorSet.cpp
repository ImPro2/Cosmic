#include "cspch.hpp"
#include "VulkanDescriptorSet.hpp"
#include "VulkanDescriptorPool.hpp"
#include "VulkanDevice.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanImage.hpp"
#include "VulkanShader.hpp"

CS_MODULE_LOG_INFO(Cosmic, Impl.RendererAPI.Vulkan.VulkanDescriptorSet);

namespace Cosmic
{

	VulkanDescriptorSet::VulkanDescriptorSet(const Ref<VulkanDevice>& device, const Ref<VulkanDescriptorPool>& descriptorPool, VkDescriptorSet handle)
		: mDevice(device), mDescriptorPool(descriptorPool), mHandle(handle)
	{
	}

	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
	}

	void VulkanDescriptorSet::SetDescriptorBindings(const Vector<VulkanDescriptorBinding>& bindings)
	{
		Vector<VkDescriptorBufferInfo> descriptorBufferInfos;
		Vector<VkDescriptorImageInfo>  descriptorImageInfos;
		Vector<VkWriteDescriptorSet>   writeDescriptorSets;

		descriptorBufferInfos.reserve(bindings.size());
		descriptorImageInfos.reserve(bindings.size());
		writeDescriptorSets.reserve(bindings.size());

		for (VulkanDescriptorBinding descriptorBinding : bindings)
		{
			VkWriteDescriptorSet writeDescriptorSet = {
				.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.dstSet          = mHandle,
				.dstBinding      = descriptorBinding.Binding,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType  = EShaderResourceTypeToVkDescriptorType(descriptorBinding.Type),
			};

			if (Ref<VulkanBuffer> buffer = descriptorBinding.Buffer.Own())
			{
				descriptorBufferInfos.push_back(VkDescriptorBufferInfo {
					.buffer = buffer->GetHandle(),
					.offset = 0,
					.range  = buffer->GetDeviceMemory()->GetMemoryRequirements().size,
				});

				writeDescriptorSet.pBufferInfo = &descriptorBufferInfos[descriptorBufferInfos.size() - 1];
			}
			else if (Ref<VulkanImage> image = descriptorBinding.Image.Own())
			{
				descriptorImageInfos.push_back(VkDescriptorImageInfo {
					.sampler     = image->GetSamplerHandle(),
					.imageView   = image->GetImageViewHandle(),
					.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				});

				writeDescriptorSet.pImageInfo = &descriptorImageInfos[descriptorImageInfos.size() - 1];
			}
			else
			{
				CS_ASSERT(false, "Invalid descriptor binding");
			}

			writeDescriptorSets.push_back(writeDescriptorSet);
		}

		vkUpdateDescriptorSets(mDevice->GetHandle(), writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);
	}

}