#pragma once
#include "ECS/Entity.hpp"
#include "Time/Time.hpp"
#include "Time/DeltaTime.hpp"
#include "Memory/SmartPtrs.hpp"

namespace Cosmic
{

    class NativeScriptEngine;

    class NativeScript : public IRefCounted
    {
    public:
        NativeScript(Entity entity)
            : mEntity(entity)
        {
        }

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
        virtual void OnInstantiate() { }
        virtual void OnDestroy()     { }
        virtual void OnUpdate(Dt dt) { }

    protected:
        Entity mEntity;

    private:
        friend class NativeScriptEngine;
        friend class Scene;
    };

    typedef Ref<NativeScript> (*InstantiateNativeScriptCallback)(Entity entity);

}
