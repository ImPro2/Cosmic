#pragma once
#include "App/Path.hpp"
#include "App/OS.hpp"
#include "ECS/Entity.hpp"
#include "Memory/SmartPtrs.hpp"

#include "Script/NativeScript.hpp"

namespace Cosmic
{

	class NativeScriptEngine
	{
	public:
		static void Init(const Path& scriptAssemblyPath = "");
		static void Shutdown();

	public:
		static void OnUpdate(Dt dt);

	public:
		static void RegisterScriptClass(const String& className);

		static Ref<NativeScript> InstantiateScriptInstance(const String& className, Entity entity);
		static void              DestroyScriptInstance(Ref<NativeScript>& instance);

	private:
		static void LoadScriptAssembly();

	private:
		inline static Path  sScriptAssemblyPath;
		inline static void* sScriptAssembly;

		inline static UnorderedMap<String, InstantiateNativeScriptCallback> sCallbackMap;       // class name - callbacks
		inline static Vector<Ref<NativeScript>>                             sScriptInstances;
	};

}
