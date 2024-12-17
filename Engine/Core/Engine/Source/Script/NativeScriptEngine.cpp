#include "cspch.hpp"
#include "NativeScriptEngine.hpp"

#include "ECS/Components.hpp"
#include "Project/ProjectManager.hpp"
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
		if (!sInstance->mActiveScene)
			return;

		sInstance->mActiveScene->ForEach<NativeScriptComponent>([](Entity entity, NativeScriptComponent& nsc)
		{
			if (!nsc.Instance && nsc.ShouldLoad)
			{
                nsc.Instance   = InstantiateScriptInstance(nsc.ClassName, entity);
                nsc.ShouldLoad = false;
			}

            if (nsc.Instance)
				nsc.Instance->OnUpdate(Time::GetDeltaTime());
		});
	}

	void NativeScriptEngine::SetActiveScene(const Ref<Scene>& scene)
	{
		sInstance->mActiveScene = scene;
	}

	void NativeScriptEngine::RegisterScriptClass(const String& className)
	{
		sInstance->mCallbackMap[className] = (InstantiateNativeScriptCallback)OS::RetrieveFunctionFromDynamicLibrary(
			GetInstantiateScriptFunctionNameFromScriptClass(className).c_str(),
			sInstance->mScriptAssembly
		);
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
		sInstance->mScriptAssemblyFile = scriptAssemblyPath;

		if (sInstance->mScriptAssemblyFile.GetAbsolutePath().GetString().empty())
			return;

		// Unload current script assembly

		if (sInstance->mScriptAssembly)
			OS::FreeDynamicLibrary(sInstance->mScriptAssembly);

		// Copy script assembly

		Path binDir  = ProjectManager::GetActiveProject()->GetParentPath() / ProjectManager::GetActiveProject()->GetInfo().BinaryDirectory;
		Path copyDir = binDir / "Runtime";

		if (!FileSystem::Exists(copyDir))
			FileSystem::CreateDirectory(copyDir);

		for (File entry : FileSystem::ListDirectoryContents(FileSystem::GetParentDirectory(sInstance->mScriptAssemblyFile)))
		{
			Path copyTo = binDir / entry.GetNameAndExtension();
			FileSystem::CopyFile(entry, copyTo);

			if (entry.GetAbsolutePath() == sInstance->mScriptAssemblyFile.GetAbsolutePath())
				sInstance->mCopiedScriptAssemblyFile = copyTo;
		}

		sInstance->mScriptAssembly = OS::LoadDynamicLibrary(sInstance->mCopiedScriptAssemblyFile);
		CS_ASSERT(sInstance->mScriptAssembly, "Unable to load script assembly {}", sInstance->mCopiedScriptAssemblyFile.GetString().c_str());

		auto initFn = (void(*)(Application*))OS::RetrieveFunctionFromDynamicLibrary(
			GetInitFunctionName().c_str(),
			sInstance->mScriptAssembly
		);

		initFn(Application::Get());
	}

	void NativeScriptEngine::ReloadScriptAssembly()
	{
		LoadScriptAssembly(sInstance->mScriptAssemblyFile);

		for (auto& [scriptClass, instantiateCallback] : sInstance->mCallbackMap)
		{
			instantiateCallback = (InstantiateNativeScriptCallback)OS::RetrieveFunctionFromDynamicLibrary(
				GetInstantiateScriptFunctionNameFromScriptClass(scriptClass).c_str(),
				sInstance->mScriptAssembly
			);
		}
	}

	String NativeScriptEngine::GetInitFunctionName()
	{
		return "CSInit";
	}

	String NativeScriptEngine::GetInstantiateScriptFunctionNameFromScriptClass(const String& className)
	{
		return std::format("CSInstantiate{}", className.c_str());
	}

}
