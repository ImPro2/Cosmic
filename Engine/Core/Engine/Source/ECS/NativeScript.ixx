module;
#include "cspch.hpp"
export module Cosmic.ECS.NativeScript;

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

    protected:
        virtual void OnCreate()      { }
        virtual void OnDestroy()     { }
        virtual void OnUpdate(Dt dt) { }

    private:
        Entity mEntity;

    private:
        friend class Scene;
    };

}