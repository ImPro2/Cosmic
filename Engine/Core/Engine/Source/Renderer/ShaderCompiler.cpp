#include "cspch.hpp"
#include "ShaderCompiler.hpp"
#include "Shader.hpp"
#include "App/File.hpp"
#include "App/FileSystem.hpp"

//#include <spirv_cross/spirv_cross.hpp>
//#include <spirv_cross/spirv_reflect.hpp>

#include "spirv_reflect.h"
#include <shaderc/shaderc.hpp>
#include <spirv-tools/linker.hpp>

CS_MODULE_LOG_INFO(Cosmic, Renderer.ShaderCompiler);

namespace Cosmic
{

	namespace Utils
	{

		static EShaderDataType SpvReflectFormatToEShaderDataType(SpvReflectFormat format)
		{
			switch (format)
			{
				case SPV_REFLECT_FORMAT_R32_UINT:            return EShaderDataType::UInt;
				case SPV_REFLECT_FORMAT_R32_SINT:            return EShaderDataType::Int;
				case SPV_REFLECT_FORMAT_R32_SFLOAT:          return EShaderDataType::Float;
				case SPV_REFLECT_FORMAT_R32G32_UINT:         return EShaderDataType::UInt2;
				case SPV_REFLECT_FORMAT_R32G32_SINT:         return EShaderDataType::Int2;
				case SPV_REFLECT_FORMAT_R32G32_SFLOAT:       return EShaderDataType::Float2;
				case SPV_REFLECT_FORMAT_R32G32B32_UINT:      return EShaderDataType::UInt3;
				case SPV_REFLECT_FORMAT_R32G32B32_SINT:      return EShaderDataType::Int3;
				case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT:    return EShaderDataType::Float3;
				case SPV_REFLECT_FORMAT_R32G32B32A32_UINT:   return EShaderDataType::UInt4;
				case SPV_REFLECT_FORMAT_R32G32B32A32_SINT:   return EShaderDataType::Int4;
				case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT: return EShaderDataType::Float4;
			}

			return EShaderDataType::Unknown;
		}

		EShaderResourceType SpvReflectDescriptorTypeToEShaderResourceType(SpvReflectDescriptorType type)
		{
			switch (type)
			{
				case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER:                    return EShaderResourceType::Sampler;
				case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:     return EShaderResourceType::CombinedImageSampler;
				case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE:              return EShaderResourceType::Unknown;
				case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE:              return EShaderResourceType::Unknown;
				case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:       return EShaderResourceType::Unknown;
				case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:       return EShaderResourceType::Unknown;
				case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:             return EShaderResourceType::UniformBuffer;
				case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:             return EShaderResourceType::Unknown;
				case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:     return EShaderResourceType::Unknown;
				case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:     return EShaderResourceType::Unknown;
				case SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:           return EShaderResourceType::InputAttachment;
				case SPV_REFLECT_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR: return EShaderResourceType::Unknown;
			}

			return EShaderResourceType::Unknown;
		}

		EShaderStage SpvReflectShaderStageToEShaderStage(SpvReflectShaderStageFlagBits stage)
		{
			switch (stage)
			{
				case SPV_REFLECT_SHADER_STAGE_VERTEX_BIT:   return EShaderStage::Vertex;
				case SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT: return EShaderStage::Fragment;
			}

			return EShaderStage::None;
		}

		enum class EShaderPreProcessorMacroType : int16
		{
			None = -1,
			ShaderStage
		};

		struct ShaderPreProcessorMacro
		{
			EShaderPreProcessorMacroType Type;
			String                       Name;
		};

		static ShaderPreProcessorMacro sPreProcessorMacros[] = {
			{ EShaderPreProcessorMacroType::ShaderStage, "#stage" },
		};

		static bool ParsePreProcessorMacro(const String& line, EShaderPreProcessorMacroType& outPreProcessorMacroType, String& outValue)
		{
			Vector<String> splitLine = StringUtils::Split(line, ' ');
			CS_ASSERT(splitLine.size() == 2, "Invalid shader preprocessor macro");

			const String name  = StringUtils::TrimCopy(splitLine[0]);
			const String value = StringUtils::TrimCopy(splitLine[1]);

			for (uint32 i = 0; i < CS_ARRAY_SIZE(sPreProcessorMacros); i++)
			{
				const ShaderPreProcessorMacro& preProcessorMacro = sPreProcessorMacros[i];

				if (preProcessorMacro.Name == name)
				{
					outPreProcessorMacroType = preProcessorMacro.Type;
					outValue                 = value;

					return true;
				}
			}

			return false;
		}

		static EShaderStage EShaderStageFromStr(const char* str)
		{
			if (strcmp(str, "vertex") == 0)
				return EShaderStage::Vertex;
			else if (strcmp(str, "fragment") == 0)
				return EShaderStage::Fragment;

			return EShaderStage::None;
		}

		static shaderc_shader_kind EShaderStageToShaderCShaderKind(EShaderStage stage)
		{
			switch (stage)
			{
				case EShaderStage::Vertex:   return shaderc_vertex_shader;
				case EShaderStage::Fragment: return shaderc_fragment_shader;
			}

			return (shaderc_shader_kind)(-1);
		}

	}

	ShaderSourceMap ShaderCompiler::PreProcess(const String& src)
	{
		ShaderSourceMap srcMap;

		Vector<String> lines = StringUtils::SplitInLines(src);

		// shader stage - line it
		using ShaderStageLineMap = UnorderedMap<EShaderStage, Vector<String>::const_iterator>;
		ShaderStageLineMap shaderStageLineMap;

		for (Vector<String>::const_iterator it = lines.begin(); it != lines.end(); it++)
		{
			const String& line = *it;

			if (line[0] != '#')
				continue;

			Utils::EShaderPreProcessorMacroType macroType;
			String value;

			if (!Utils::ParsePreProcessorMacro(line, macroType, value))
				continue;

			switch (macroType)
			{
				case Utils::EShaderPreProcessorMacroType::ShaderStage:
				{
					shaderStageLineMap[Utils::EShaderStageFromStr(value.c_str())] = it;
					break;
				}
			}
		}

		for (ShaderStageLineMap::const_iterator it = shaderStageLineMap.begin(); it != shaderStageLineMap.end(); it++)
		{
			EShaderStage stage = it->first;

			Vector<String>::const_iterator lineIt     = it->second;
			Vector<String>::const_iterator nextLineIt = std::next(it) != shaderStageLineMap.end() ? std::next(it)->second : lines.end();

			// Shader source starts after #stage macro

			Vector<String>::const_iterator start = std::next(lineIt);
			Vector<String>::const_iterator end   = nextLineIt != lines.end() ? std::prev(nextLineIt) : nextLineIt;

			Vector<String> stageSrcLines = Vector<String>(start, end);

			srcMap[stage] = StringUtils::Concatenate(stageSrcLines);
		}

		return srcMap;
	}

	Vector<uint32> ShaderCompiler::LinkSpirv(const Vector<Vector<uint32>>& spirvs)
	{
		spvtools::Context context(SPV_ENV_VULKAN_1_3);

		context.SetMessageConsumer([](spv_message_level_t level, const char*, const spv_position_t& pos, const char* msg)
		{
			ELogSeverity severity;

			switch (level)
			{
				case SPV_MSG_FATAL:          severity = ELogSeverity::Critical; break;
				case SPV_MSG_INTERNAL_ERROR: severity = ELogSeverity::Error;    break;
				case SPV_MSG_ERROR:          severity = ELogSeverity::Error;    break;
				case SPV_MSG_WARNING:        severity = ELogSeverity::Warn;     break;
				case SPV_MSG_INFO:           severity = ELogSeverity::Info;     break;
			}

			Log::LogWithSeverity("SPIRV Shader Linking ({}:{}): {}", severity, pos.line, pos.column, msg);
		});

		Vector<uint32> linkedBinary;
		spv_result_t result = spvtools::Link(context, spirvs, &linkedBinary);

		if (result != SPV_SUCCESS)
		{
			return {};
		}

		return linkedBinary;
	}

	Vector<uint32> ShaderCompiler::CompileToSpirv(const String& shaderName, EShaderSourceLanguage language, EShaderStage stage, const String& src)
	{
		shaderc::Compiler compiler;
		shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(src, Utils::EShaderStageToShaderCShaderKind(stage), shaderName.c_str());

		String msg = result.GetErrorMessage();

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			CS_LOG_ERROR("Shader Compilation Error ({}): {}", shaderName.c_str(), msg.c_str());
			return {};
		}
		else if (result.GetNumWarnings() > 0)
		{
			CS_LOG_WARN("Shader Compilation Warning ({}): {}", shaderName.c_str(), msg.c_str());
		}

		return Vector<uint32>(result.begin(), result.end());
	}

	SpirvBinary ShaderCompiler::CompileToSpirv(const String& shaderName, EShaderSourceLanguage language, const ShaderSourceMap& srcMap)
	{
		Vector<Vector<uint32>> spirvs;

		for (const auto& [stage, src] : srcMap)
		{
			spirvs.push_back(CompileToSpirv(shaderName, language, stage, src));
		}

		Vector<uint32> linkedBinary = LinkSpirv(spirvs);

		SpirvBinary spirv;
		spirv.Binary = Buffer((uint8*)linkedBinary.data(), linkedBinary.size() * 4);
		return spirv;
	}

	ShaderReflectionData ShaderCompiler::Reflect(const SpirvBinary& spirv)
	{
		ShaderReflectionData reflectionData;

		SpvReflectShaderModule shaderModule;
		SpvReflectResult result = spvReflectCreateShaderModule(spirv.Binary.GetSize(), spirv.Binary.GetData(), &shaderModule);

		for (uint32 i = 0; i < shaderModule.entry_point_count; i++)
		{
			const SpvReflectEntryPoint& entryPoint = shaderModule.entry_points[i];

			ShaderStageMetadata shaderStageMetadata = {
				.Stage          = Utils::SpvReflectShaderStageToEShaderStage(entryPoint.shader_stage),
				.EntryPointName = entryPoint.name
			};

			reflectionData.ShaderStages.push_back(shaderStageMetadata);

			if (entryPoint.shader_stage & SPV_REFLECT_SHADER_STAGE_VERTEX_BIT)
			{
				for (uint32 j = 0; j < entryPoint.input_variable_count; j++)
				{
					const SpvReflectInterfaceVariable* inputVariable = entryPoint.input_variables[j];

					uint32 location = inputVariable->location;
					EShaderDataType dataType = Utils::SpvReflectFormatToEShaderDataType(inputVariable->format);
					bool normalized = !(inputVariable->decoration_flags & SPV_REFLECT_DECORATION_FLAT);

					reflectionData.InputLayout.AddAttribute(GraphicsPipelineInputLayoutAttribute(location, dataType, normalized));
				}
			}

			for (uint32 j = 0; j < entryPoint.descriptor_set_count; j++)
			{
				const SpvReflectDescriptorSet& descriptorSet = entryPoint.descriptor_sets[j];

				ShaderDescriptorSetMetadata* descriptorSetMetadata = nullptr;

				for (uint32 k = 0; k < reflectionData.DescriptorSets.size(); k++)
				{
					if (descriptorSet.set == k)
					{
						descriptorSetMetadata = &reflectionData.DescriptorSets[k];
						break;
					}
				}

				if (!descriptorSetMetadata) // New set
				{
					reflectionData.DescriptorSets.push_back({});
					descriptorSetMetadata = &reflectionData.DescriptorSets[reflectionData.DescriptorSets.size() - 1];
				}

				for (uint32 k = 0; k < descriptorSet.binding_count; k++)
				{
					const SpvReflectDescriptorBinding* descriptorBinding = descriptorSet.bindings[k];

					ShaderResourceMetadata resourceMetadata = {
						.Binding      = descriptorBinding->binding,
						.Set          = descriptorBinding->set,
						.Name         = descriptorBinding->name,
						.Type         = Utils::SpvReflectDescriptorTypeToEShaderResourceType(descriptorBinding->descriptor_type),
						.ShaderStages = Utils::SpvReflectShaderStageToEShaderStage(entryPoint.shader_stage)
					};

					descriptorSetMetadata->Bindings.push_back(resourceMetadata);
				}
			}

			for (uint32 j = 0; j < shaderModule.push_constant_block_count; j++)
			{
				const SpvReflectBlockVariable& constantBlock = shaderModule.push_constant_blocks[j];

				ShaderPushConstantMetadata pushConstantMetadata = {
					.Offset = constantBlock.offset,
					.Name   = constantBlock.name
				};

				reflectionData.PushConstants.push_back(pushConstantMetadata);
			}
		}

		return reflectionData;
	}

}