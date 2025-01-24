#include "cspch.hpp"
#include "VulkanGraphicsPipeline.hpp"

#include "VulkanShader.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanDevice.hpp"

namespace Cosmic
{

	namespace Utils
	{

		static VkFormat EShaderDataTypeToVkFormat(EShaderDataType dataType)
		{
			switch (dataType)
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

		static VkPrimitiveTopology EPrimitiveTopologyToVkPrimitiveTopology(EPrimitiveTopology topology)
		{
			switch (topology)
			{
				case EPrimitiveTopology::PointList:     return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
				case EPrimitiveTopology::LineList:      return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
				case EPrimitiveTopology::LineStrip:     return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
				case EPrimitiveTopology::TriangleList:  return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				case EPrimitiveTopology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			}

			return (VkPrimitiveTopology)(-1);
		}

		static VkCullModeFlags ECullModeToVkCullModeFlags(ECullMode cullMode)
		{
			switch (cullMode)
			{
				case ECullMode::None:  return VK_CULL_MODE_NONE;
				case ECullMode::Front: return VK_CULL_MODE_FRONT_BIT;
				case ECullMode::Back:  return VK_CULL_MODE_BACK_BIT;
			}

			return VK_CULL_MODE_NONE;
		}

	}

	VulkanGraphicsPipeline::VulkanGraphicsPipeline(const GraphicsPipelineInfo& info, const Ref<VulkanRenderPass>& renderPass, const Ref<VulkanDevice>& device)
		: GraphicsPipeline(info), mRenderPass(renderPass), mDevice(device)
	{
	}

	VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
	{
		if (mPipelineHandle)
			vkDestroyPipeline(mDevice->GetHandle(), mPipelineHandle, nullptr);
	}

	void VulkanGraphicsPipeline::CreateHandle()
	{
		Vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos;

		const ShaderInfo& shaderInfo = mInfo.Shader->GetInfo();

		if (shaderInfo.Stages.IsSet(EShaderStage::Vertex))
		{
			const ShaderStageMetadata& shaderStageMetadata = mInfo.Shader->GetReflectionData().ShaderStages[0];

			shaderStageCreateInfos.push_back({
				.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.flags  = 0,
				.stage  = VK_SHADER_STAGE_VERTEX_BIT,
				.module = mInfo.Shader.As<VulkanShader>()->GetOrCreateShaderModuleHandle(),
				.pName  = shaderStageMetadata.EntryPointName.c_str()
			});
		}

		if (shaderInfo.Stages.IsSet(EShaderStage::Fragment))
		{
			const ShaderStageMetadata& shaderStageMetadata = mInfo.Shader->GetReflectionData().ShaderStages[1];

			shaderStageCreateInfos.push_back({
				.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.flags  = 0,
				.stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
				.module = mInfo.Shader.As<VulkanShader>()->GetOrCreateShaderModuleHandle(),
				.pName  = shaderStageMetadata.EntryPointName.c_str()
			});
		}

		VkVertexInputBindingDescription vertexInputBindingDescription = {
			.binding   = 0,
			.stride    = mInfo.InputLayout.GetStride(),
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
		};

		Vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions(mInfo.InputLayout.GetAttributes().size());

		for (uint32 i = 0; i < mInfo.InputLayout.GetAttributes().size(); i++)
		{
			const GraphicsPipelineInputLayoutAttribute& attribute = mInfo.InputLayout.GetAttributes()[i];

			vertexInputAttributeDescriptions[i] = {
				.location = attribute.Location,
				.binding  = 0,
				.format   = Utils::EShaderDataTypeToVkFormat(attribute.Type),
				.offset   = attribute.Offset
			};
		}

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {
			.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.flags                           = 0,
			.vertexBindingDescriptionCount   = 1,
			.pVertexBindingDescriptions      = &vertexInputBindingDescription,
			.vertexAttributeDescriptionCount = (uint32)vertexInputAttributeDescriptions.size(),
			.pVertexAttributeDescriptions    = vertexInputAttributeDescriptions.data()
		};

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
			.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.flags                  = 0,
			.topology               = Utils::EPrimitiveTopologyToVkPrimitiveTopology(mInfo.PrimitiveTopology),
			.primitiveRestartEnable = VK_FALSE,
		};

		uint2 renderPassSize = mRenderPass->GetOutputColorAttachment(0).Size;

		VkViewport viewport = {
			.width  = (float32)renderPassSize.width,
			.height = (float32)renderPassSize.height,
		};

		VkRect2D scissor = {
			.offset = {},
			.extent = { renderPassSize.width, renderPassSize.height }
		};

		VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
			.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.flags         = 0,
			.viewportCount = 1,
			.pViewports    = &viewport,
			.scissorCount  = 1,
			.pScissors     = &scissor
		};

		VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
			.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.flags                   = 0,
			.depthClampEnable        = VK_FALSE,
			.rasterizerDiscardEnable = VK_FALSE,
			.polygonMode             = VK_POLYGON_MODE_FILL,
			.cullMode                = Utils::ECullModeToVkCullModeFlags(mInfo.CullMode),
			.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE,
			.depthBiasEnable         = VK_FALSE,
			.depthBiasConstantFactor = 0.0f,
			.depthBiasSlopeFactor    = 0.0f,
			.lineWidth               = 1.0f
		};

		VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {
			.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.flags                 = 0,
			.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
			.sampleShadingEnable   = VK_FALSE,
			.minSampleShading      = 1.0f,
			.pSampleMask           = nullptr,
			.alphaToCoverageEnable = VK_FALSE,
			.alphaToOneEnable      = VK_FALSE
		};

		VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {
			.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			.flags                 = 0,
			.depthTestEnable       = mInfo.EnableDepthTesting,
			.depthWriteEnable      = mInfo.EnableDepthTesting,
			.depthCompareOp        = VK_COMPARE_OP_LESS,
			.depthBoundsTestEnable = VK_FALSE,
			.stencilTestEnable     = VK_FALSE,
			.front                 = {},
			.back                  = {},
			.minDepthBounds        = 0.0f,
			.maxDepthBounds        = 1.0f
		};

		VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
			.blendEnable         = VK_TRUE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			.colorBlendOp        = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp        = VK_BLEND_OP_ADD,
			.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
		};

		VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
			.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.flags           = 0,
			.logicOpEnable   = VK_FALSE,
			.logicOp         = {},
			.attachmentCount = 1,
			.pAttachments    = &colorBlendAttachmentState,
		};

		VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
			.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.dynamicStateCount = CS_ARRAY_SIZE(dynamicStates),
			.pDynamicStates    = dynamicStates
		};

		Vector<VkDescriptorSetLayout> descriptorSetLayouts = mInfo.Shader.As<VulkanShader>()->GetDescriptorSetLayouts();

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
			.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.flags                  = 0,
			.setLayoutCount         = (uint32)descriptorSetLayouts.size(),
			.pSetLayouts            = descriptorSetLayouts.data(),
			.pushConstantRangeCount = 0,
			.pPushConstantRanges    = nullptr
		};

		VK_CALL(vkCreatePipelineLayout(mDevice->GetHandle(), &pipelineLayoutCreateInfo, nullptr, &mPipelineLayoutHandle));

		VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
			.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.flags               = 0,
			.stageCount          = (uint32)shaderStageCreateInfos.size(),
			.pStages             = shaderStageCreateInfos.data(),
			.pVertexInputState   = &vertexInputStateCreateInfo,
			.pInputAssemblyState = &inputAssemblyStateCreateInfo,
			.pViewportState      = &viewportStateCreateInfo,
			.pRasterizationState = &rasterizationStateCreateInfo,
			.pMultisampleState   = &multisampleStateCreateInfo,
			.pDepthStencilState  = &depthStencilStateCreateInfo,
			.pColorBlendState    = &colorBlendStateCreateInfo,
			.pDynamicState       = &dynamicStateCreateInfo,
			.layout              = mPipelineLayoutHandle,
			.renderPass          = mRenderPass->GetHandle(),
			.subpass             = 0,
			.basePipelineHandle  = VK_NULL_HANDLE,
			.basePipelineIndex   = 0
		};

		VK_CALL(vkCreateGraphicsPipelines(mDevice->GetHandle(), VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &mPipelineHandle));

		mInfo.Shader.As<VulkanShader>()->DestroyShaderModuleHandle();
	}

	VkPipeline VulkanGraphicsPipeline::GetOrCreateHandle()
	{
		if (mPipelineHandle == VK_NULL_HANDLE)
			CreateHandle();

		return mPipelineHandle;
	}

}