#pragma once
#include "Shader.hpp"
#include "GraphicsPipelineInputLayout.hpp"

namespace Cosmic
{

    enum class EPrimitiveTopology
    {
        None          = -1,
        PointList     =  0,
        LineList      =  1,
        LineStrip     =  2,
        TriangleList  =  3,
        TriangleStrip =  4
    };

    enum class ECullMode
    {
        None  = 0,
        Front = 1,
        Back  = 2
    };

	struct GraphicsPipelineInfo
	{
        GraphicsPipelineInputLayout InputLayout;

        EPrimitiveTopology PrimitiveTopology;
        ECullMode          CullMode;

        bool EnableDepthTesting;

        Ref<Shader> Shader;
	};

	class GraphicsPipeline
	{
    public:
        GraphicsPipeline(const GraphicsPipelineInfo& info);
        ~GraphicsPipeline() = default;

    public:
        const GraphicsPipelineInfo& GetInfo() const { return mInfo; }

    protected:
        GraphicsPipelineInfo mInfo;
	};

}
