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

		sInstance->mRegistry.Init();

		return sInstance;
	}

	void NativeScriptEngine::Shutdown()
	{
		sInstance->mRegistry.Shutdown();

		sInstance.Release();
	}

	void NativeScriptEngine::OnUpdate(Dt dt)
	{
		if (!sInstance->mActiveScene)
			return;

		sInstance->mRegistry.OnUpdate(sInstance->mActiveScene);
	}

	void NativeScriptEngine::SetActiveScene(const Ref<Scene>& scene)
	{
		sInstance->mActiveScene = scene;
	}

	void NativeScriptEngine::LoadScriptAssembly(const Path& scriptAssemblyPath)
	{
		sInstance->mScriptAssemblyFile = scriptAssemblyPath;

		if (sInstance->mScriptAssemblyFile.GetAbsolutePath().GetString().empty())
			return;

		// Check if it's valid

		if (!FileSystem::Exists(sInstance->mScriptAssemblyFile))
		{
			CS_LOG_WARN("Attempting to load non-existent script assembly {}", sInstance->mScriptAssemblyFile.GetAbsolutePath().GetString().c_str());
			return;
		}

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
			Path copyTo = copyDir / entry.GetNameAndExtension();
			FileSystem::CopyFile(entry, copyTo);

			if (entry.GetAbsolutePath() == sInstance->mScriptAssemblyFile.GetAbsolutePath())
				sInstance->mCopiedScriptAssemblyFile = copyTo;
		}

		sInstance->mScriptAssembly = OS::LoadDynamicLibrary(sInstance->mCopiedScriptAssemblyFile);
		CS_ASSERT(sInstance->mScriptAssembly, "Unable to load script assembly");

		auto initFn = (void(*)(Application*))OS::RetrieveFunctionFromDynamicLibrary(
			sInitFunctionName.c_str(),
			sInstance->mScriptAssembly
		);

		initFn(Application::Get());

		CS_LOG_INFO("Loaded script assembly: {} (copied from: {})", sInstance->mCopiedScriptAssemblyFile.GetAbsolutePath().GetString().c_str(), sInstance->mScriptAssemblyFile.GetAbsolutePath().GetString().c_str());
	}

	void NativeScriptEngine::ReloadScriptAssembly()
	{
		sInstance->mRegistry.OnScriptAssemblyReloaded(sInstance->mActiveScene);

		LoadScriptAssembly(sInstance->mScriptAssemblyFile);
	}

}
