#pragma once
#include <entt/entt.hpp>

#include "App/Log/Log.hpp"
#include "Base/Base.hpp"

namespace Cosmic
{

    enum class EComponentType;
    struct IComponent;

    class Entity
    {
    public:
        Entity() = default;
        void operator=(const Entity& other)
        {
            mEntityHandle = other.mEntityHandle;
            mRegistry = other.mRegistry;
        }

        Entity(entt::entity handle, entt::registry* registry)
            : mEntityHandle(handle), mRegistry(registry)
        {
        }

        void AddComponent(EComponentType type, IComponent* component);
        void RemoveComponent(EComponentType type);

        IComponent* GetComponent(EComponentType type);
        bool HasComponent(EComponentType type);

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
            return mRegistry->all_of<T>(mEntityHandle);
        }

        bool operator==(const Entity& other) const
        {
            return mEntityHandle == other.mEntityHandle && mRegistry == other.mRegistry;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }

        entt::registry& GetRegistry() const { return *mRegistry; }
        
        operator bool()         const { return mEntityHandle != entt::null; }
        operator entt::entity() const { return mEntityHandle;               }
        operator uint32()       const { return (uint32)mEntityHandle;       }

    private:
        entt::entity mEntityHandle{ entt::null };
        entt::registry* mRegistry = nullptr;
    };

}
