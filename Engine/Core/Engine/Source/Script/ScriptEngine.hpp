#pragma once
#include "Base/Base.hpp"
#include "Script/NativeScript.hpp"
#include "App/OS.hpp"

namespace Cosmic
{

	class ScriptEngine
	{
	public:
		static void Init(const String& scriptAssemblyPath);
		static void Shutdown();

		static NativeScriptCallbacks AddNativeScript(const String& className);
	};

}
