module;
#include "cspch.hpp"
export module Cosmic.Script.NativeScript;

import Cosmic.ECS.Entity;
import Cosmic.Time;
import Cosmic.Time.DeltaTime;

namespace Cosmic
{

    export class NativeScript
    {
    public:
        virtual ~NativeScript() { }

    public:
        template<typename T, typename ... Args>
        void AddComponent(Args&& ... args)
        {
            mEntity.AddComponent<T, Args...>(args);
        }

        template<typename T>
        void RemoveComponent()
        {
            mEntity.RemoveComponent<T>();
        }

        template<typename T>
        T& GetComponent()
        {
            return mEntity.GetComponent<T>();
        }

        template<typename T>
        bool HasComponent()
        {
            return mEntity.HasComponent<T>();
        }

    protected:
        virtual void OnCreate()      { }
        virtual void OnDestroy()     { }
        virtual void OnUpdate(Dt dt) { }

    protected:
        Entity mEntity;

    private:
        friend class Scene;
    };

    export typedef NativeScript* (*InstantiateNativeScriptCallback)();
    export typedef void(*DestroyNativeScriptCallback)(NativeScript*);

    export struct NativeScriptCallbacks
    {
        InstantiateNativeScriptCallback InstantiateScript;
        DestroyNativeScriptCallback     DestroyScript;
    };

}