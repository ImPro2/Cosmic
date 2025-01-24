#pragma once
#include "Base/Base.hpp"
#include "ShaderTypes.hpp"
#include "GraphicsPipelineInputLayout.hpp"

namespace Cosmic
{

	struct ShaderStageMetadata
	{
		EShaderStage  Stage;
		String        EntryPointName;
	};

	enum class EShaderResourceType : int16
	{
		Unknown = -1,
		Sampler,
		CombinedImageSampler,
		InputAttachment,
		UniformBuffer
	};

	struct ShaderResourceMetadata
	{
		uint32                 Binding = 0;
		uint32                 Set     = 0;
		String                 Name    = "";
		EShaderResourceType    Type    = EShaderResourceType::UniformBuffer;
		BitFlags<EShaderStage> ShaderStages;
	};

	struct ShaderDescriptorSetMetadata
	{
		Vector<ShaderResourceMetadata> Bindings;
	};

	struct ShaderPushConstantMetadata
	{
		uint32 Offset = 0;
		String Name   = "";
	};

	struct ShaderReflectionData
	{
		GraphicsPipelineInputLayout         InputLayout;
		Vector<ShaderStageMetadata>         ShaderStages;
		Vector<ShaderDescriptorSetMetadata> DescriptorSets;
		Vector<ShaderPushConstantMetadata>  PushConstants;
	};

}
