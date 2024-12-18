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

}