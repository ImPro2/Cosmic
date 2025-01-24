#include "cspch.hpp"
#include "GraphicsPipelineInputLayout.hpp"

namespace Cosmic
{

	namespace Utils
	{

		static uint32 EShaderDataTypeToSize(EShaderDataType type)
		{
			switch (type)
			{
				case EShaderDataType::Float:  return sizeof(float32) * 1;
				case EShaderDataType::Float2: return sizeof(float32) * 2;
				case EShaderDataType::Float3: return sizeof(float32) * 3;
				case EShaderDataType::Float4: return sizeof(float32) * 4;
				case EShaderDataType::Int:    return sizeof(int32) * 1;
				case EShaderDataType::Int2:   return sizeof(int32) * 2;
				case EShaderDataType::Int3:   return sizeof(int32) * 3;
				case EShaderDataType::Int4:   return sizeof(int32) * 4;
				case EShaderDataType::Mat2:   return sizeof(float32) * 2 * 2;
				case EShaderDataType::Mat3:   return sizeof(float32) * 3 * 3;
				case EShaderDataType::Mat4:   return sizeof(float32) * 4 * 4;
				case EShaderDataType::Bool:   return sizeof(bool);
			}
		}

		static uint32 EShaderDataTypeComponentCount(EShaderDataType type)
		{
			switch (type)
			{
				case EShaderDataType::Float:  return 1;
				case EShaderDataType::Float2: return 2;
				case EShaderDataType::Float3: return 3;
				case EShaderDataType::Float4: return 4;
				case EShaderDataType::Int:    return 1;
				case EShaderDataType::Int2:   return 2;
				case EShaderDataType::Int3:   return 3;
				case EShaderDataType::Int4:   return 4;
				case EShaderDataType::Mat2:   return 2;
				case EShaderDataType::Mat3:   return 3;
				case EShaderDataType::Mat4:   return 4;
				case EShaderDataType::Bool:   return 1;
			}
		}

	}

	GraphicsPipelineInputLayoutAttribute::GraphicsPipelineInputLayoutAttribute(uint32 location, EShaderDataType type, bool normalized /*= false*/)
		: Location(location), ComponentCount(Utils::EShaderDataTypeComponentCount(type)), Type(type), Normalized(normalized), Offset(0), Size(Utils::EShaderDataTypeToSize(type))
	{
	}

	GraphicsPipelineInputLayout::GraphicsPipelineInputLayout(std::initializer_list<GraphicsPipelineInputLayoutAttribute> attributes)
		: mAttributes(attributes), mStride(0)
	{
		CalculateOffsetsAndStride();
	}

	void GraphicsPipelineInputLayout::AddAttribute(const GraphicsPipelineInputLayoutAttribute& attribute)
	{
		mAttributes.push_back(attribute);
		CalculateOffsetsAndStride();
	}

	void GraphicsPipelineInputLayout::CalculateOffsetsAndStride()
	{
        uint32 offset = 0;
		mStride       = 0;

		for (GraphicsPipelineInputLayoutAttribute& attribute : mAttributes)
		{
			attribute.Offset = offset;

			offset  += attribute.Size;
			mStride += attribute.Size;
		}
	}

}