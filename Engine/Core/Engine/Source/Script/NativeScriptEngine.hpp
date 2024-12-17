#pragma once
#include "App/File.hpp"
#include "App/OS.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Scene.hpp"
#include "Memory/SmartPtrs.hpp"

#include "Script/NativeScript.hpp"

namespace Cosmic
{

	class Application;

	class NativeScriptEngine : public IRefCounted
	{
	private:
		static PersistentRef<NativeScriptEngine> Init(const Path& scriptAssemblyPath);
		static void                              Shutdown();

	public:
		static void OnUpdate(Dt dt);

		static void SetActiveScene(const Ref<Scene>& scene);

	public:
		static const File& GetScriptAssemblyFile() { return sInstance->mScriptAssemblyFile; }

	public:
		static void RegisterScriptClass(const String& className);

		static Ref<NativeScript> InstantiateScriptInstance(const String& className, Entity entity);
		static void              DestroyScriptInstance(Ref<NativeScript>& instance);

		static void LoadScriptAssembly(const Path& scriptAssemblyPath = "");
		static void ReloadScriptAssembly();

	private:
		static String GetInitFunctionName();
		static String GetInstantiateScriptFunctionNameFromScriptClass(const String& className);

	private:
		File  mScriptAssemblyFile;
		File  mCopiedScriptAssemblyFile;
		void* mScriptAssembly;

		UnorderedMap<String, InstantiateNativeScriptCallback> mCallbackMap;       // class name - callbacks
		Vector<Ref<NativeScript>>                             mScriptInstances;

		Ref<Scene> mActiveScene;

		inline static PersistentRef<NativeScriptEngine> sInstance;

		friend class Application;
	};

}
