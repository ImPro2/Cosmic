#pragma once
#include "Script/NativeScript.hpp"
#include "Script/Field.hpp"
#include "Memory/SmartPtrs.hpp"
#include "ECS/Scene.hpp"
#include "Base/Random.hpp"
#include "ECS/Entity.hpp"

namespace Cosmic
{

	using NativeScriptInstantiateCallbackMap = UnorderedMap<String, InstantiateNativeScriptCallback>;
	using NativeScriptFieldMap = UnorderedMap<NativeScriptID, Vector<IField*>>;

	class NativeScriptEngine;

	class NativeScriptRegistry
	{
	public:
		template<class T>
		T* AllocateNativeScript(Entity entity)
		{
			mLastInstantiatedScriptID = Random<int32>(0, std::numeric_limits<int32>::max());

			T* instance   = DefaultAllocator::Allocate<T>(entity);
			instance->mID = mLastInstantiatedScriptID;

			return instance;
		}

	private:
		void Init();
		void Shutdown();

		void OnUpdate(const Ref<Scene>& scene);

	public:
		void RegisterScriptClass(const String& className);

		Ref<NativeScript> InstantiateScript(const String& className, Entity entity);
		void              DestroyScriptInstance(Ref<NativeScript>& instance);

	public:
		void ReleaseScriptInstances(const Ref<Scene>& scene);
		void ReloadInstantiateCallbacks();

	public:
		NativeScriptInstantiateCallbackMap&       GetCallbackMap()       { return mCallbackMap; }
		const NativeScriptInstantiateCallbackMap& GetCallbackMap() const { return mCallbackMap; }

		NativeScriptFieldMap&       GetFieldMap()       { return mFieldMap; }
		const NativeScriptFieldMap& GetFieldMap() const { return mFieldMap; }

		Vector<IField*>&       GetScriptInstanceFields(const Ref<NativeScript>& instance)       { return mFieldMap[instance->mID];    }
		const Vector<IField*>& GetScriptInstanceFields(const Ref<NativeScript>& instance) const { return mFieldMap.at(instance->mID); }

		Vector<Ref<NativeScript>>&       GetInstances()       { return mScriptInstances; }
		const Vector<Ref<NativeScript>>& GetInstances() const { return mScriptInstances; }

	private:
		void RegisterField(IField* field);

	private:
		NativeScriptInstantiateCallbackMap mCallbackMap;       // class name - callbacks
		NativeScriptFieldMap               mFieldMap;          // script instance - fields

		Vector<Ref<NativeScript>>          mScriptInstances;

		NativeScriptID mLastInstantiatedScriptID = -1;

		friend class NativeScriptEngine;
		friend class IField;
	};

}
