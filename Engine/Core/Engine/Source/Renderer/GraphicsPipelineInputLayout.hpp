#pragma once
#include "Base/Base.hpp"
#include "ShaderTypes.hpp"

namespace Cosmic
{

	struct GraphicsPipelineInputLayoutAttribute
    {
        GraphicsPipelineInputLayoutAttribute() = default;
        GraphicsPipelineInputLayoutAttribute(uint32 location, EShaderDataType type, bool normalized = false);

        uint32          Location;
        uint32          ComponentCount;
        EShaderDataType Type;
        bool            Normalized;
        uint32          Offset;
        uint32          Size; // NOT stride; just the size of the attribute.

    };

    class GraphicsPipelineInputLayout
    {
    public:
        GraphicsPipelineInputLayout() = default;
        GraphicsPipelineInputLayout(std::initializer_list<GraphicsPipelineInputLayoutAttribute> attributes);

    public:
        void AddAttribute(const GraphicsPipelineInputLayoutAttribute& attribute);

    public:
        const Vector<GraphicsPipelineInputLayoutAttribute>& GetAttributes() const { return mAttributes; }

        uint32 GetStride() const { return mStride; }

    private:
        void CalculateOffsetsAndStride();

    private:
        Vector<GraphicsPipelineInputLayoutAttribute> mAttributes;

        uint32 mStride;
    };

}
