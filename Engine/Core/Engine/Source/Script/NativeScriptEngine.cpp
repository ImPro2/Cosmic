#include "cspch.hpp"
#include "NativeScriptEngine.hpp"

#include "App/Application.hpp"

CS_MODULE_LOG_INFO(Cosmic, Script.NativeScriptEngine);

namespace Cosmic
{

	PersistentRef<NativeScriptEngine> NativeScriptEngine::Init(const Path& scriptAssemblyPath)
	{
		sInstance = CreatePersistentRef<NativeScriptEngine>();

		LoadScriptAssembly(scriptAssemblyPath);

		return sInstance;
	}

	void NativeScriptEngine::Shutdown()
	{
		sInstance.Release();
	}

	void NativeScriptEngine::OnUpdate(Dt dt)
	{
		for (Ref<NativeScript> instance : sInstance->mScriptInstances)
			instance->OnUpdate(dt);
	}

	void NativeScriptEngine::RegisterScriptClass(const String& className)
	{
		String instantiateFunctionName = std::format("CSInstantiate{}", className.c_str());

		sInstance->mCallbackMap[className] = (InstantiateNativeScriptCallback)OS::RetrieveFunctionFromDynamicLibrary(instantiateFunctionName.c_str(), sInstance->mScriptAssembly);
	}

	Ref<NativeScript> NativeScriptEngine::InstantiateScriptInstance(const String& className, Entity entity)
	{
		if (sInstance->mCallbackMap.find(className) == sInstance->mCallbackMap.end())
			RegisterScriptClass(className);

		Ref<NativeScript> instance = Ref<NativeScript>(sInstance->mCallbackMap[className](entity));
		instance->OnInstantiate();

		sInstance->mScriptInstances.push_back(instance);

		return instance;
	}

	void NativeScriptEngine::DestroyScriptInstance(Ref<NativeScript>& instance)
	{
		instance->OnDestroy();

		auto it = std::find(sInstance->mScriptInstances.begin(), sInstance->mScriptInstances.end(), instance);
		sInstance->mScriptInstances.erase(it);

		instance.Release();
	}

	void NativeScriptEngine::LoadScriptAssembly(const Path& scriptAssemblyPath)
	{
		sInstance->mScriptAssemblyPath = scriptAssemblyPath;

		if (sInstance->mScriptAssemblyPath.GetString().empty())
			return;

		sInstance->mScriptAssembly = OS::LoadDynamicLibrary(sInstance->mScriptAssemblyPath);

		CS_ASSERT(sInstance->mScriptAssembly, "Unable to load script assembly {}", sInstance->mScriptAssemblyPath.GetString().c_str());

		auto initFn = (void(*)(Application*))OS::RetrieveFunctionFromDynamicLibrary("CSInit", sInstance->mScriptAssembly);
		initFn(Application::Get());
	}

}
