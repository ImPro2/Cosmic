#include "cspch.hpp"
#include "VulkanShader.hpp"
#include "VulkanDevice.hpp"

namespace Cosmic
{

	namespace Utils
	{

		static VkFormat EShaderDataTypeToVkFormat(EShaderDataType type)
		{
			switch (type)
			{
				case Cosmic::EShaderDataType::Float:  return VK_FORMAT_R32_SFLOAT;
				case Cosmic::EShaderDataType::Float2: return VK_FORMAT_R32G32_SFLOAT;
				case Cosmic::EShaderDataType::Float3: return VK_FORMAT_R32G32B32_SFLOAT;
				case Cosmic::EShaderDataType::Float4: return VK_FORMAT_R32G32B32A32_SFLOAT;
				case Cosmic::EShaderDataType::UInt:   return VK_FORMAT_R32_UINT;
				case Cosmic::EShaderDataType::UInt2:  return VK_FORMAT_R32G32_UINT;
				case Cosmic::EShaderDataType::UInt3:  return VK_FORMAT_R32G32B32_UINT;
				case Cosmic::EShaderDataType::UInt4:  return VK_FORMAT_R32G32B32A32_UINT;
				case Cosmic::EShaderDataType::Int:    return VK_FORMAT_R32_SINT;  
				case Cosmic::EShaderDataType::Int2:   return VK_FORMAT_R32G32_SINT;
				case Cosmic::EShaderDataType::Int3:   return VK_FORMAT_R32G32B32_SINT;
				case Cosmic::EShaderDataType::Int4:   return VK_FORMAT_R32G32B32A32_SINT;
			}

			return VK_FORMAT_UNDEFINED;
		}

	}

	VkShaderStageFlags EShaderStagesToVkShaderStageFlags(BitFlags<EShaderStage> shaderStages)
	{
		VkShaderStageFlags flags = 0;

		if (shaderStages.IsSet(EShaderStage::Vertex))
			flags |= VK_SHADER_STAGE_VERTEX_BIT;

		if (shaderStages.IsSet(EShaderStage::Fragment))
			flags |= VK_SHADER_STAGE_FRAGMENT_BIT;

		return flags;
	}

	VkDescriptorType EShaderResourceTypeToVkDescriptorType(EShaderResourceType type)
	{
		switch (type)
		{
			case EShaderResourceType::Sampler:              return VK_DESCRIPTOR_TYPE_SAMPLER;
			case EShaderResourceType::CombinedImageSampler: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			case EShaderResourceType::InputAttachment:      return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
			case EShaderResourceType::UniformBuffer:        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		}

		return (VkDescriptorType)(-1);
	}

	VulkanShader::VulkanShader(const ShaderInfo& info, const Ref<VulkanDevice>& device)
		: Shader(info), mDevice(device)
	{
	}

	VulkanShader::~VulkanShader()
	{
	}

	const VkVertexInputBindingDescription& VulkanShader::GetVertexInputBindingDescription()
	{
		static bool generated = false;

		if (!generated)
		{
			mVertexInputBindingDescription = {
				.binding   = 0,
				.stride    = mReflectionData.InputLayout.GetStride(),
				.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
			};
		}

		return mVertexInputBindingDescription;
	}

	const Vector<VkVertexInputAttributeDescription>& VulkanShader::GetVertexInputAttributeDescriptions()
	{
		static bool generated = false;

		if (!generated)
		{
			for (const GraphicsPipelineInputLayoutAttribute& attribute : mReflectionData.InputLayout.GetAttributes())
			{
				VkVertexInputAttributeDescription vertexInputAttributeDescription = {
					.location = attribute.Location,
					.binding  = 0,
					.format   = Utils::EShaderDataTypeToVkFormat(attribute.Type),
					.offset   = attribute.Offset
				};

				mVertexInputAttributeDescriptions.push_back(vertexInputAttributeDescription);
			}
		}

		return mVertexInputAttributeDescriptions;
	}

	const Vector<VkDescriptorSetLayout>& VulkanShader::GetDescriptorSetLayouts()
	{
		static bool generated = false;

		if (!generated)
		{
			mDescriptorSetLayouts.resize(mReflectionData.DescriptorSets.size());

			for (uint32 i = 0; i < mReflectionData.DescriptorSets.size(); i++)
			{
				const Vector<ShaderResourceMetadata>& descriptorBindings = mReflectionData.DescriptorSets[i].Bindings;

				Vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings(descriptorBindings.size());

				for (uint32 j = 0; j < descriptorBindings.size(); j++)
				{
					const ShaderResourceMetadata& resourceMetadata = descriptorBindings[j];

					descriptorSetLayoutBindings[j] = {
						.binding            = resourceMetadata.Binding,
						.descriptorType     = EShaderResourceTypeToVkDescriptorType(resourceMetadata.Type),
						.descriptorCount    = 1,
						.stageFlags         = EShaderStagesToVkShaderStageFlags(resourceMetadata.ShaderStages),
						.pImmutableSamplers = nullptr
					};
				}

				VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
					.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
					.flags        = 0,
					.bindingCount = (uint32)descriptorSetLayoutBindings.size(),
					.pBindings    = descriptorSetLayoutBindings.data()
				};

				VK_CALL(vkCreateDescriptorSetLayout(mDevice->GetHandle(), &descriptorSetLayoutCreateInfo, nullptr, &mDescriptorSetLayouts[i]));
			}
		}

		return mDescriptorSetLayouts;
	}

	void VulkanShader::CreateShaderModuleHandle()
	{
		VkShaderModuleCreateInfo shaderModuleCreateInfo = {
			.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.codeSize = (uint32)mBinary.Binary.GetSize(),
			.pCode    = (uint32*)mBinary.Binary.GetData()
		};

		VK_CALL(vkCreateShaderModule(mDevice->GetHandle(), &shaderModuleCreateInfo, nullptr, &mShaderModuleHandle));
	}

	void VulkanShader::DestroyShaderModuleHandle()
	{
		vkDestroyShaderModule(mDevice->GetHandle(), mShaderModuleHandle, nullptr);
	}

	VkShaderModule VulkanShader::GetOrCreateShaderModuleHandle()
	{
		if (mShaderModuleHandle == VK_NULL_HANDLE)
			CreateShaderModuleHandle();

		return mShaderModuleHandle;
	}

}