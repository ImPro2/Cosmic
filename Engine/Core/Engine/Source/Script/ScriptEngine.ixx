module;
#include "cspch.hpp"
export module Cosmic.Script.ScriptEngine;

import Cosmic.Base;
import Cosmic.Script.NativeScript;
import Cosmic.App.OS;

namespace Cosmic
{

	export class ScriptEngine
	{
	public:
		static void Init(const String& scriptAssemblyPath);
		static void Shutdown();

		static NativeScriptCallbacks AddNativeScript(const String& className);
	};

}
