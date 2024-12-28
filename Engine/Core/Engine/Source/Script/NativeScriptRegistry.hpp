#pragma once
#include "Script/NativeScript.hpp"
#include "Script/Field.hpp"
#include "Memory/Memory.hpp"
#include "Base/Random.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Scene.hpp"

namespace Cosmic
{

	using NativeScriptInstantiateCallbackMap = UnorderedMap<String, InstantiateNativeScriptCallback>;
	using NativeScriptFieldMap = UnorderedMap<NativeScriptID, Vector<IField*>>;

	using EnumStringConversionCallbackMap = UnorderedMap<String, Pair<EnumToStringCallback, EnumFromStringCallback>>;
	using EnumStringMap = UnorderedMap<String, Vector<String>>;

	class Scene;
	class NativeScriptEngine;

	class NativeScriptRegistry
	{
	public:
		// Functions called from script dll

		template<class T>
		T* AllocateNativeScript(Entity entity)
		{
			if (mNextScriptID == -1)
				mLastInstantiatedScriptID = Random<int32>(0, std::numeric_limits<int32>::max());
			else
			{
				mLastInstantiatedScriptID = mNextScriptID;
				mNextScriptID             = -1;
			}

			T* instance   = DefaultAllocator::Allocate<T>(entity);
			instance->mID = mLastInstantiatedScriptID;

			return instance;
		}

	private:
		void Init();
		void Shutdown();

		void OnRuntimeStart(const Ref<Scene>& scene);
		void OnRuntimeStop(const Ref<Scene>& scene);
		void OnRuntimeUpdate(const Ref<Scene>& scene);

	public:
		void RegisterScriptClass(const String& className, InstantiateNativeScriptCallback callback = nullptr);
		void RegisterEnumClass(const String& enumClass, const String& toStrFunctionName, const String& fromStrFunctionName);

		Ref<NativeScript> InstantiateScript(const String& className, Entity entity);
		Ref<NativeScript> InstantiateRuntimeScript(const String& className, Entity entity, const Ref<NativeScript>& instance);
		void              DestroyScriptInstance(Ref<NativeScript>& instance);

		void InstantiateScriptInstances(const Ref<Scene>& scene);

		Ref<NativeScript> FindScriptByID(int32 id);
		Ref<NativeScript> FindRuntimeScriptByID(int32 id);

	private:
		void OnScriptAssemblyUnloaded();

	private:
		void ClearScriptInstancesEntities();

		void ReleaseScriptInstances();
		void ClearRegisteredScriptClasses();
		void ClearRegisteredEnumClasses();

	public:
		Vector<Ref<NativeScript>>&       GetInstances()       { return mScriptInstances; }
		const Vector<Ref<NativeScript>>& GetInstances() const { return mScriptInstances; }

		Vector<Ref<NativeScript>>&       GetRuntimeInstances()       { return mRuntimeScriptInstances; }
		const Vector<Ref<NativeScript>>& GetRuntimeInstances() const { return mRuntimeScriptInstances; }

		NativeScriptFieldMap&       GetFieldMap()       { return mFieldMap; }
		const NativeScriptFieldMap& GetFieldMap() const { return mFieldMap; }

		NativeScriptFieldMap&       GetRuntimeFieldMap()       { return mRuntimeFieldMap; }
		const NativeScriptFieldMap& GetRuntimeFieldMap() const { return mRuntimeFieldMap; }

		NativeScriptInstantiateCallbackMap&       GetCallbackMap()       { return mCallbackMap; }
		const NativeScriptInstantiateCallbackMap& GetCallbackMap() const { return mCallbackMap; }

		EnumStringConversionCallbackMap&       GetEnumStringConversionCallbackMap()       { return mEnumConversionCallbackMap; }
		const EnumStringConversionCallbackMap& GetEnumStringConversionCallbackMap() const { return mEnumConversionCallbackMap; }

		EnumStringMap&       GetEnumStringMap()       { return mEnumStringMap; }
		const EnumStringMap& GetEnumStringMap() const { return mEnumStringMap; }

		Vector<String>&       GetRegisteredClassNames()       { return mRegisteredClassNames; }
		const Vector<String>& GetRegisteredClassNames() const { return mRegisteredClassNames; }

		Vector<IField*>&       GetScriptInstanceFields(const Ref<NativeScript>& instance)       { return mFieldMap[instance->mID];    }
		const Vector<IField*>& GetScriptInstanceFields(const Ref<NativeScript>& instance) const { return mFieldMap.at(instance->mID); }

		Vector<IField*>&       GetRuntimeScriptInstanceFields(const Ref<NativeScript>& instance)       { return mRuntimeFieldMap[instance->mID];    }
		const Vector<IField*>& GetRuntimeScriptInstanceFields(const Ref<NativeScript>& instance) const { return mRuntimeFieldMap.at(instance->mID); }

	private:
		void RegisterField(IField* field);

	private:
		bool mIsRuntime = false;

		Vector<Ref<NativeScript>>          mScriptInstances;
		Vector<Ref<NativeScript>>          mRuntimeScriptInstances;

		NativeScriptFieldMap               mFieldMap;                  // script instance - fields
		NativeScriptFieldMap               mRuntimeFieldMap;           // script instance - fields

		NativeScriptInstantiateCallbackMap mCallbackMap;               // class name - callbacks
		Vector<String>                     mRegisteredClassNames;      // keys of mCallbackMap

		EnumStringConversionCallbackMap    mEnumConversionCallbackMap; // enum type name - callbacks
		EnumStringMap                      mEnumStringMap;             // enum type name - strings of values

		NativeScriptID                     mNextScriptID             = -1;
		NativeScriptID                     mLastInstantiatedScriptID = -1;

		friend class NativeScriptEngine;
		friend class IField;
	};

}
