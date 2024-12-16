#include "cspch.hpp"
#include "NativeScriptEngine.hpp"

CS_MODULE_LOG_INFO(Cosmic, Script.NativeScriptEngine);

namespace Cosmic
{

	void NativeScriptEngine::Init(const Path& scriptAssemblyPath)
	{
		LoadScriptAssembly(scriptAssemblyPath);


	}

	void NativeScriptEngine::Shutdown()
	{
	}

	void NativeScriptEngine::OnUpdate(Dt dt)
	{
		for (Ref<NativeScript> instance : sScriptInstances)
			instance->OnUpdate(dt);
	}

	void NativeScriptEngine::RegisterScriptClass(const String& className)
	{
		String instantiateFunctionName = std::format("CSInstantiate{}", className.c_str());

		sCallbackMap[className] = (InstantiateNativeScriptCallback)OS::RetrieveFunctionFromDynamicLibrary(instantiateFunctionName.c_str(), sScriptAssembly);
	}

	Ref<NativeScript> NativeScriptEngine::InstantiateScriptInstance(const String& className, Entity entity)
	{

		if (sCallbackMap.find(className) == sCallbackMap.end())
			RegisterScriptClass(className);

		Ref<NativeScript> instance = Ref<NativeScript>(sCallbackMap[className](entity));
		instance->OnInstantiate();

		sScriptInstances.push_back(instance);

		return instance;
	}

	void NativeScriptEngine::DestroyScriptInstance(Ref<NativeScript>& instance)
	{
		instance->OnDestroy();

		auto it = std::find(sScriptInstances.begin(), sScriptInstances.end(), instance);
		sScriptInstances.erase(it);

		instance.Release();
	}

	void NativeScriptEngine::LoadScriptAssembly(const Path& scriptAssemblyPath)
	{
		sScriptAssemblyPath = scriptAssemblyPath;

		if (sScriptAssemblyPath.GetString().empty())
			return;

		sScriptAssembly = OS::LoadDynamicLibrary(sScriptAssemblyPath);

		CS_ASSERT(sScriptAssembly, "Unable to load script assembly {}", sScriptAssemblyPath.GetString().c_str());

		auto initFn = (void(*)(Application*))OS::RetrieveFunctionFromDynamicLibrary("CSInit", sScriptAssembly);
		initFn(Application::Get());
	}

}
