#pragma once
#include "Base/Types.hpp"
#include "Base/Tuples.hpp"
#include "Base/Macros.hpp"


#define CS_DECLARE_TYPE_STR(T) template<> String TypeStr< T >()
#define CS_DEFINE_TYPE_STR(T)  template<> String TypeStr< T >() { return CS_STRINGIFY(T); }

namespace Cosmic
{

	template<typename T>
	String TypeStr()
	{
		// Hack for other types

		Vector<String> result = StringUtils::Split(String(typeid(T).name()), ':');
		return result[result.size() - 1];
	}

	CS_DECLARE_TYPE_STR(float32);
	CS_DECLARE_TYPE_STR(float2);
	CS_DECLARE_TYPE_STR(float3);
	CS_DECLARE_TYPE_STR(float4);
	CS_DECLARE_TYPE_STR(int32);
	CS_DECLARE_TYPE_STR(int2);
	CS_DECLARE_TYPE_STR(int3);
	CS_DECLARE_TYPE_STR(int4);
	CS_DECLARE_TYPE_STR(uint32);
	CS_DECLARE_TYPE_STR(uint2);
	CS_DECLARE_TYPE_STR(uint3);
	CS_DECLARE_TYPE_STR(uint4);
	CS_DECLARE_TYPE_STR(String);

}
