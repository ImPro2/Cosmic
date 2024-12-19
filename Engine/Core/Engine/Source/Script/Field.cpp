#include "cspch.hpp"
#include "Field.hpp"

#include "Script/NativeScriptEngine.hpp"
#include "Script/NativeScriptRegistry.hpp"

namespace Cosmic
{

	void IField::RegisterField()
	{
		NativeScriptRegistry& registry = NativeScriptEngine::GetRegistry();
		registry.RegisterField(this);
	}

	String IField::GetEnumToStringFunctionName()
	{
		return std::format("{}ToStr", GetTypeName());
	}

	String IField::GetEnumFromStringFunctionName()
	{
		return std::format("{}FromStr", GetTypeName());
	}

}