#include "cspch.hpp"
#include "Field.hpp"

#include "Script/NativeScriptEngine.hpp"
#include "Script/NativeScriptRegistry.hpp"

namespace Cosmic
{

	void IField::RegisterField()
	{
		if (!mRegistered)
		{
			NativeScriptRegistry& registry = NativeScriptEngine::GetRegistry();
			registry.RegisterField(this);

			mRegistered = true;
		}
	}

	String IField::GetEnumToStringFunctionName()
	{
		return std::format("{}ToStr", GetTypeName());
	}

	String IField::GetEnumFromStringFunctionName()
	{
		return std::format("{}FromStr", GetTypeName());
	}

	void IField::CopyFrom(IField* other)
	{
		if (mType != other->mType)
			return;

		switch (mType)
		{
			case EFieldType::Float32: SetValue(other->GetValue<float32>()); SetDefaultValue(other->GetDefaultValue<float32>()); break;
			case EFieldType::Float2:  SetValue(other->GetValue<float2>());  SetDefaultValue(other->GetDefaultValue<float2>());  break;
			case EFieldType::Float3:  SetValue(other->GetValue<float3>());  SetDefaultValue(other->GetDefaultValue<float3>());  break;
			case EFieldType::Float4:  SetValue(other->GetValue<float4>());  SetDefaultValue(other->GetDefaultValue<float4>());  break;
			case EFieldType::Int32:   SetValue(other->GetValue<int32>());   SetDefaultValue(other->GetDefaultValue<int32>());   break;
			case EFieldType::Int2:    SetValue(other->GetValue<int2>());    SetDefaultValue(other->GetDefaultValue<int2>());    break;
			case EFieldType::Int3:    SetValue(other->GetValue<int3>());    SetDefaultValue(other->GetDefaultValue<int3>());    break;
			case EFieldType::Int4:    SetValue(other->GetValue<int4>());    SetDefaultValue(other->GetDefaultValue<int4>());    break;
			case EFieldType::UInt32:  SetValue(other->GetValue<uint32>());  SetDefaultValue(other->GetDefaultValue<uint32>());  break;
			case EFieldType::UInt2:   SetValue(other->GetValue<uint2>());   SetDefaultValue(other->GetDefaultValue<uint2>());   break;
			case EFieldType::UInt3:   SetValue(other->GetValue<uint3>());   SetDefaultValue(other->GetDefaultValue<uint3>());   break;
			case EFieldType::UInt4:   SetValue(other->GetValue<uint4>());   SetDefaultValue(other->GetDefaultValue<uint4>());   break;
			case EFieldType::String:  SetValue(other->GetValue<String>());  SetDefaultValue(other->GetDefaultValue<String>());  break;
			case EFieldType::Enum:    SetValue(other->GetValue<int16>());   SetDefaultValue(other->GetDefaultValue<int16>());   break;
		}
	}

}