#pragma once
#include "Base/Base.hpp"

namespace Cosmic
{

    enum class EShaderDataType : int16
    {
		Unknown = -1,
        Float, Float2, Float3, Float4,
        UInt,  UInt2,  UInt3,  UInt4,
        Int,   Int2,   Int3,   Int4,
        Mat2,  Mat3,   Mat4,
        Bool
    };

	enum class EShaderStage : int16
	{
        None     = 0,
		Vertex   = CS_BIT(0),
		Fragment = CS_BIT(1),
        All      = Vertex | Fragment
	};

	enum class EShaderSourceLanguage
	{
		GLSL,
		HLSL
	};

}
