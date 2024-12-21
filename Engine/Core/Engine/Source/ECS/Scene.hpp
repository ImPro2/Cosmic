#pragma once
#include "Time/DeltaTime.hpp"
#include "Time/Time.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Renderer/Camera.hpp" 
#include "Base/Base.hpp"
#include "ECS/SceneCamera.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Components.hpp"
#include "Renderer/OrthographicCamera.hpp"
#include "ECS/SceneCamera.hpp"
#include "Memory/Memory.hpp"

#include <entt/entt.hpp>
#include <entt/entity/fwd.hpp>

namespace Cosmic
{

    class Scene : public IRefCounted
    {
    public:
        Scene();
        ~Scene();

    public:
        void OnUpdate(Dt dt);
        void OnUpdateEditor(Dt dt, const Camera& camera, const glm::mat4& cameraTransform);
        void OnViewportResize(uint32 width, uint32 height);

    public:
        Entity CreateEntity(const String& name = "", Entity parent = Entity());
        Entity CreateSerializedEntity(const EntityMetadataComponent& metadata);
        Entity AddEntity(Entity entity);
        void   RemoveEntity(Entity entity);

        Entity FindEntityByTag(const String& tag);
        Entity FindEntityByID(int32 id);
        Entity FindRootParent(Entity entity);

        void ForEachRootEntity(std::function<void(Entity)> fn);
        void ForEachEntity(std::function<void(Entity)> fn);
        void ForEachEntityIndexed(std::function<void(Entity, int32)> fn);
        void ForEachChild(Entity parent, std::function<void(Entity)> fn);
        void ForEachChild(const EntityMetadataComponent& parentMetadata, std::function<void(Entity)> fn);
        void ForEachChildRecurse(Entity parent, std::function<void(Entity)> fn);
        void ForEachChildRecurse(const EntityMetadataComponent& parentMetadata, std::function<void(Entity)> fn);

        template<typename... T, typename F>
        void ForEach(F callback)
        {
            mRegistry.view<T...>().each([&](entt::entity entity, T&... components)
			{
				callback(Entity{ entity, &mRegistry }, std::forward<T&>(components)...);
			});
        }

        size_t GetEntityCount() const;
        entt::registry* GetRegistryPtr() { return &mRegistry; }

    private:
        void ResolveRelativeChildProperties();
        void ResolveRelativeChildPropertiesRecurse(Entity entity);

    private:
        entt::registry mRegistry;
        EntityMetadataComponent mSceneRootMetadata;

    private:
        friend class Entity;
    };

}
