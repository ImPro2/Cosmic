#pragma once
#include "Base/Base.hpp"
#include "ECS/Entity.hpp"
#include "Time/Time.hpp"
#include "Time/DeltaTime.hpp"
#include "Memory/Memory.hpp"

#include "Script/NativeScriptMacros.hpp"
#include "Script/Field.hpp"

namespace Cosmic
{

    using NativeScriptID = int32;

    class NativeScriptRegistry;

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
        virtual void OnSceneStart()  { }
        virtual void OnSceneStop()   { }
        virtual void OnUpdate(Dt dt) { }

    protected:
        Entity         mEntity;
        NativeScriptID mID = -1;

    private:
        friend class NativeScriptRegistry;
        friend class Scene;
    };

    typedef NativeScript* (*InstantiateNativeScriptCallback)(Entity entity);

}
