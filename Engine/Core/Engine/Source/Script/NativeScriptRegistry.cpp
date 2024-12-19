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

            if (Ref<NativeScript> instance = nsc.Instance.Own())
				instance->OnUpdate(Time::GetDeltaTime());
		});
	}

	void NativeScriptRegistry::RegisterScriptClass(const String& className, InstantiateNativeScriptCallback callback)
	{
		if (!callback)
		{
			mCallbackMap[className] = (InstantiateNativeScriptCallback)OS::RetrieveFunctionFromDynamicLibrary(
				Utils::GetInstantiateScriptFunctionNameFromScriptClass(className).c_str(),
				NativeScriptEngine::GetLoadedScriptAssembly()
			);
		}
		else
		{
			mCallbackMap[className] = callback;
		}

		mRegisteredClassNames.push_back(className);

		CS_LOG_DEBUG("Registered script class {}", className.c_str());
	}

	void NativeScriptRegistry::RegisterEnumClass(const String& enumClass, const String& toStrFunctionName, const String& fromStrFunctionName)
	{
		void* scriptAssembly = NativeScriptEngine::GetLoadedScriptAssembly();

		EnumToStringCallback   toStrCallback   = static_cast<const char* (*)(int16)>(OS::RetrieveFunctionFromDynamicLibrary(toStrFunctionName.c_str(),   scriptAssembly));
		EnumFromStringCallback fromStrCallback = static_cast<int16 (*)(const char*)>(OS::RetrieveFunctionFromDynamicLibrary(fromStrFunctionName.c_str(), scriptAssembly));

		mEnumConversionCallbackMap[enumClass] = { toStrCallback, fromStrCallback };

		Vector<String> stringValues;

		for (int16 i = 0;; i++)
		{
			String valueStr = toStrCallback(i);

			if (valueStr == "Last")
				break;

			stringValues.push_back(valueStr);
		}

		mEnumStringMap[enumClass] = stringValues;

		CS_LOG_DEBUG("Registered enum class {}", enumClass.c_str());
	}

	Ref<NativeScript> NativeScriptRegistry::InstantiateScript(const String& className, Entity entity)
	{
		if (mCallbackMap.find(className) == mCallbackMap.end())
			RegisterScriptClass(className);

		Ref<NativeScript> instance = Ref<NativeScript>(mCallbackMap[className](entity));
		instance->OnInstantiate();

		mScriptInstances.push_back(instance);

		CS_LOG_DEBUG("Instantiated script {}", className.c_str());

		return instance;
	}

	void NativeScriptRegistry::DestroyScriptInstance(Ref<NativeScript>& instance)
	{
		instance->OnDestroy();

		mFieldMap[instance].clear();
		mFieldMap.erase(instance);

		auto it = std::find(mScriptInstances.begin(), mScriptInstances.end(), instance);
		mScriptInstances.erase(it);

		CS_LOG_DEBUG("Destroyed script instance");

		instance.Release();
	}

	void NativeScriptRegistry::OnScriptAssemblyReloaded(const Ref<Scene>& scene)
	{
		OnScriptAssemblyUnloaded();
		SetUnloadedScriptInstancesToLoad(scene);
	}

	void NativeScriptRegistry::OnScriptAssemblyUnloaded()
	{
		ReleaseScriptInstances();
		ClearRegisteredScriptClasses();
		ClearRegisteredEnumClasses();

		mLastInstantiatedScriptID = -1;
	}

	void NativeScriptRegistry::ReleaseScriptInstances()
	{
		for (Ref<NativeScript>& instance : mScriptInstances)
		{
			instance->OnDestroy();
			mFieldMap[instance].clear();
			instance.Release();
		}

		mFieldMap.clear();
		mScriptInstances.clear();
	}

	void NativeScriptRegistry::ClearRegisteredScriptClasses()
	{
		mCallbackMap.clear();
		mRegisteredClassNames.clear();
	}

	void NativeScriptRegistry::ClearRegisteredEnumClasses()
	{
		mEnumConversionCallbackMap.clear();
		mEnumStringMap.clear();
	}

	void NativeScriptRegistry::SetUnloadedScriptInstancesToLoad(const Ref<Scene>& scene)
	{
		scene->ForEach<NativeScriptComponent>([](Entity entity, NativeScriptComponent& nsc)
		{
			if (!nsc.Instance)
				nsc.ShouldLoad = true;
		});
	}

	void NativeScriptRegistry::RegisterField(IField* field)
	{
		if (mLastInstantiatedScriptID == -1)
		{
			CS_LOG_WARN("Unable to register field due to invalid script instance");
			return;
		}

		mFieldMap[mLastInstantiatedScriptID].push_back(field);

		if (field->GetType() == EFieldType::Enum && mEnumConversionCallbackMap.find(field->GetTypeName()) == mEnumConversionCallbackMap.end())
		{
			RegisterEnumClass(field->GetTypeName(), field->GetEnumToStringFunctionName(), field->GetEnumFromStringFunctionName());
		}

		CS_LOG_DEBUG("Registered field {} ({})", field->GetName(), field->GetTypeName());
	}

}