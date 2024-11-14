#pragma once
#include "ECS/Entity.hpp"
#include "Time/Time.hpp"
#include "Time/DeltaTime.hpp"

namespace Cosmic
{

    class NativeScript
    {
    public:
        virtual ~NativeScript() { }

    public:
        template<typename T, typename ... Args>
        void AddComponent(Args&& ... args)
        {
            mEntity.AddComponent<T, Args...>(args...);
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

    typedef NativeScript* (*InstantiateNativeScriptCallback)();
    typedef void(*DestroyNativeScriptCallback)(NativeScript*);

    struct NativeScriptCallbacks
    {
        InstantiateNativeScriptCallback InstantiateScript;
        DestroyNativeScriptCallback     DestroyScript;
    };

}
