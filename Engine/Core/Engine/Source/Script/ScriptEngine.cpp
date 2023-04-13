module;
#include "cspch.hpp"
module Cosmic.Script.ScriptEngine;

import Cosmic.App.Application;

namespace Cosmic
{

	struct ScriptEngineData
	{
		String ScriptAssemblyPath;
		void* ScriptAssembly; // The Script DLL
	};

	static ScriptEngineData* sScriptingData = nullptr;

	void ScriptEngine::Init(const String& scriptAssemblyPath)
	{
		sScriptingData = new ScriptEngineData();

		sScriptingData->ScriptAssemblyPath = scriptAssemblyPath;
		sScriptingData->ScriptAssembly = OS::LoadDynamicLibrary(sScriptingData->ScriptAssemblyPath.c_str());

		const char* scriptInitFnName = "CSInit";
		typedef void(*ScriptInitFn)(Application*);

		ScriptInitFn scriptInitFn = (ScriptInitFn)OS::RetrieveFunctionFromDynamicLibrary(scriptInitFnName, sScriptingData->ScriptAssembly);

		scriptInitFn(Application::Get());
	}

	void ScriptEngine::Shutdown()
	{
		OS::FreeDynamicLibrary(sScriptingData->ScriptAssembly);

		delete sScriptingData;
		sScriptingData = nullptr;
	}
	
	NativeScriptCallbacks ScriptEngine::AddNativeScript(const String& className)
	{
		// Retrieve instantiate and destroy functions associated with class name.

		String instantiateFunctionName = std::format("CSInstantiate{}", className.c_str());
		String destroyFunctionName     = std::format("CSDestroy{}", className.c_str());

		NativeScriptCallbacks callbacks = {};
		callbacks.InstantiateScript = (InstantiateNativeScriptCallback)OS::RetrieveFunctionFromDynamicLibrary(instantiateFunctionName.c_str(), sScriptingData->ScriptAssembly);
		callbacks.DestroyScript     = (DestroyNativeScriptCallback)OS::RetrieveFunctionFromDynamicLibrary(destroyFunctionName.c_str(), sScriptingData->ScriptAssembly);

		return callbacks;
	}


}
