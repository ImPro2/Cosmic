#pragma once
#include "App/File.hpp"
#include "App/OS.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Scene.hpp"
#include "Memory/Memory.hpp"

#include "Script/NativeScript.hpp"
#include "Script/NativeScriptRegistry.hpp"

namespace Cosmic
{

	class Application;

	class NativeScriptEngine : public IRefCounted
	{
	public:
		template<class T>
		static T* AllocateNativeScript(Entity entity)
		{
			return sInstance->mRegistry.AllocateNativeScript<T>(entity);
		}

	private:
		static PersistentRef<NativeScriptEngine> Init(const Path& scriptAssemblyPath);
		static void                              Shutdown();

	public:
		static void OnRuntimeStart();
		static void OnRuntimeStop();
		static void OnRuntimeUpdate(Dt dt);

		static void SetActiveScene(const Ref<Scene>& scene);

	public:
		static const File&           GetScriptAssemblyFile()   { return sInstance->mScriptAssemblyFile; }
		static NativeScriptRegistry& GetRegistry()             { return sInstance->mRegistry;           }
		static void*                 GetLoadedScriptAssembly() { return sInstance->mScriptAssembly;     }

	public:
		static void LoadScriptAssembly(const Path& scriptAssemblyPath = "");
		static void ReloadScriptAssembly();

	private:
		File  mScriptAssemblyFile;
		File  mCopiedScriptAssemblyFile;
		void* mScriptAssembly;

		NativeScriptRegistry mRegistry;

		Ref<Scene> mActiveScene;

		inline static PersistentRef<NativeScriptEngine> sInstance;
		inline static String sInitFunctionName = "CSInit";

		friend class Application;
	};

}
