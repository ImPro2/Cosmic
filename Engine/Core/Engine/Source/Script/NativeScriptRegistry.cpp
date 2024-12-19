#include "cspch.hpp"
#include "NativeScriptRegistry.hpp"
#include "Script/Field.hpp"
#include "Script/NativeScriptEngine.hpp"

#include "ECS/Components.hpp"

CS_MODULE_LOG_INFO(Cosmic, Script.NativeScriptRegistry);

namespace Cosmic
{

	namespace Utils
	{

		static String GetInstantiateScriptFunctionNameFromScriptClass(const String& className)
		{
			return std::format("CSInstantiate{}", className.c_str());
		}

	}

	void NativeScriptRegistry::Init()
	{

	}

	void NativeScriptRegistry::Shutdown()
	{

	}

	void NativeScriptRegistry::OnUpdate(const Ref<Scene>& scene)
	{
		scene->ForEach<NativeScriptComponent>([this](Entity entity, NativeScriptComponent& nsc)
		{
			if (!nsc.Instance && nsc.ShouldLoad)
			{
                nsc.Instance   = InstantiateScript(nsc.ClassName, entity);
                nsc.ShouldLoad = false;
			}

            if (nsc.Instance)
				nsc.Instance->OnUpdate(Time::GetDeltaTime());
		});
	}

	void NativeScriptRegistry::RegisterScriptClassStr(const String& className)
	{
		mCallbackMap[className] = (InstantiateNativeScriptCallback)OS::RetrieveFunctionFromDynamicLibrary(
			Utils::GetInstantiateScriptFunctionNameFromScriptClass(className).c_str(),
			NativeScriptEngine::GetLoadedScriptAssembly()
		);

		mRegisteredClassNames.push_back(className);
	}

	Ref<NativeScript> NativeScriptRegistry::InstantiateScript(const String& className, Entity entity)
	{
		if (mCallbackMap.find(className) == mCallbackMap.end())
			RegisterScriptClassStr(className);

		Ref<NativeScript> instance = Ref<NativeScript>(mCallbackMap[className](entity));
		instance->OnInstantiate();

		mScriptInstances.push_back(instance);

		return instance;
	}

	void NativeScriptRegistry::DestroyScriptInstance(Ref<NativeScript>& instance)
	{
		instance->OnDestroy();

		auto it = std::find(mScriptInstances.begin(), mScriptInstances.end(), instance);
		mScriptInstances.erase(it);

		instance.Release();
	}

	void NativeScriptRegistry::ReleaseScriptInstances(const Ref<Scene>& scene)
	{
		for (Ref<NativeScript>& instance : mScriptInstances)
		{
			instance->OnDestroy();
			instance.Release();
		}

		mScriptInstances.clear();

		scene->ForEach<NativeScriptComponent>([](Entity entity, NativeScriptComponent& nsc)
		{
			nsc.ShouldLoad = true;
			nsc.Instance   = nullptr;
		});
	}

	void NativeScriptRegistry::ReloadInstantiateCallbacks()
	{
		for (auto& [scriptClass, instantiateCallback] : mCallbackMap)
		{
			RegisterScriptClassStr(scriptClass);
		}
	}

	void NativeScriptRegistry::RegisterField(IField* field)
	{
		if (mLastInstantiatedScriptID == -1)
		{
			CS_LOG_WARN("Unable to register field due to invalid script instance");
			return;
		}

		mFieldMap[mLastInstantiatedScriptID].push_back(field);
	}

}