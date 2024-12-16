#pragma once
#include "App/Path.hpp"
#include "App/OS.hpp"
#include "ECS/Entity.hpp"
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

	public:
		static void RegisterScriptClass(const String& className);

		static Ref<NativeScript> InstantiateScriptInstance(const String& className, Entity entity);
		static void              DestroyScriptInstance(Ref<NativeScript>& instance);

		static void LoadScriptAssembly(const Path& scriptAssemblyPath = "");

	private:
		Path  mScriptAssemblyPath;
		void* mScriptAssembly;

		UnorderedMap<String, InstantiateNativeScriptCallback> mCallbackMap;       // class name - callbacks
		Vector<Ref<NativeScript>>                             mScriptInstances;

		inline static PersistentRef<NativeScriptEngine> sInstance;

		friend class Application;
	};

}
