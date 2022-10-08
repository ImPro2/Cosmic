module;
#include "cspch.hpp"
#include <entt/entt.hpp>
export module Cosmic.ECS.Entity;

import Cosmic.App.Log;
import Cosmic.Base;

namespace Cosmic
{

    export class Entity
    {
    public:
        Entity() = default;
        void operator=(const Entity& other)
        {
            mEntityHandle = other.mEntityHandle;
            mRegistry     = other.mRegistry;
        }
        Entity(entt::entity handle, entt::registry* registry)
            : mEntityHandle(handle), mRegistry(registry)
        {
        }

        template<typename T, typename ... Args>
        T& AddComponent(Args&& ... args)
        {
            //CS_ASSERT(!HasComponent<T>(), "This entity already has this component.");
            return mRegistry->emplace<T>(mEntityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        void RemoveComponent()
        {
            //CS_ASSERT(HasComponent<T>(), "This entity does not have this component.");
            mRegistry->remove<T>(mEntityHandle);
        }

        template<typename T>
        T& GetComponent()
        {
            //CS_ASSERT(HasComponent<T>(), "This entity does not have this component yet.");
            return mRegistry->get<T>(mEntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return mRegistry->has<T>(mEntityHandle);
        }

        bool operator==(const Entity& other) const
        {
            return mEntityHandle == other.mEntityHandle && mRegistry == other.mRegistry;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }
        
        operator bool()         const { return mEntityHandle != entt::null; }
        operator entt::entity() const { return mEntityHandle;               }
        operator uint32()       const { return (uint32)mEntityHandle;       }

    private:
        entt::entity mEntityHandle{ entt::null };
        entt::registry* mRegistry = nullptr;
    };

}