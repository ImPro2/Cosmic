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
        Ref<Scene> Copy();

    public:
        void OnRuntimeStart();
        void OnRuntimeStop();

        void OnRuntimeUpdate(Dt dt);
        void OnEditorUpdate(Dt dt);

        void OnViewportResize(uint32 width, uint32 height);

    public:
        void RenderMainCamera();
        void RenderCamera(const Camera& camera, const glm::mat4& cameraTransform);

    public:
        Entity CreateEntity(const String& name = "", Entity parent = Entity());
        Entity AddEntity(Entity entity);
        void   RemoveEntity(Entity entity);

        void RegisterEntity(Entity entity, EntityMetadataComponent& parentMetadata);
        void UnregisterEntity(Entity entity, bool releaseChildren = true);

        Entity CreateSerializedEntity(const EntityMetadataComponent& metadata, EntityIDMetadataMap& idMetadataMap);
        Entity CreateSerializedEntity(const EntityMetadataComponent& metadata, const EntityIDMetadata& idMetadata, EntityIDMetadataMap& idMetadataMap);
        void   RegisterSerializedEntity(Entity entity);
        void   RegisterSerializedEntities(const EntityIDMetadataMap& idMetadataMap);

        void ReparentEntity(Entity entity, Entity parent);

        Entity FindEntityByTag(const String& tag);
        Entity FindEntityByID(int32 id);
        Entity FindRootParent(Entity entity);
        Entity FindMainCameraEntity();

        void ForEachRootEntity(std::function<void(Entity)> fn);
        void ForEachEntity(std::function<void(Entity)> fn);
        void ForEachEntityIndexed(std::function<void(Entity, int32)> fn);
        void ForEachChild(Entity parent, std::function<void(Entity)> fn);
        void ForEachChild(const EntityMetadataComponent& parentMetadata, std::function<void(Entity)> fn);
        void ForEachChildRecurseTopDown(Entity parent, std::function<void(Entity)> fn);
        void ForEachChildRecurseTopDown(const EntityMetadataComponent& parentMetadata, std::function<void(Entity)> fn);
        void ForEachChildRecurseBottomUp(Entity parent, std::function<void(Entity)> fn);
        void ForEachChildRecurseBottomUp(const EntityMetadataComponent& parentMetadata, std::function<void(Entity)> fn);

        template<typename... Ts, typename F>
        void ForEach(ComponentGroup<Ts...>, F callback)
        {
            (ForEach<Ts>(callback), ...);
        }

        template<typename... T, typename F>
        void ForEach(F callback)
        {
            mRegistry.view<T...>().each([&](entt::entity entity, T&... components)
			{
				callback(Entity{ entity, &mRegistry }, std::forward<T&>(components)...);
			});
        }

    public:
        size_t GetEntityCount() const;
        entt::registry* GetRegistryPtr() { return &mRegistry; }

        EntityMetadataComponent& GetSceneRootMetadata() { return mSceneRootMetadata; }

    private:
        void ResolveRelativeChildProperties();
        void ResolveRelativeChildPropertiesRecurse(Entity entity);

    private:
        void CopyAllComponents(Entity from, Entity to);

        template<typename... Ts>
        void CopySceneComponents(ComponentGroup<Ts...>, const Ref<Scene>& other, EntityIDMetadataMap& idMetadataMap)
        {
            ([&]()
			{
				mRegistry.view<EntityMetadataComponent, Ts>().each([&](entt::entity entity, EntityMetadataComponent& metadata, Ts& component)
				{
                    Entity srcEntity = Entity{ entity, &mRegistry };
                    Entity dstEntity = other->FindEntityByID(metadata.ID);

                    if (!dstEntity)
                    {
                        //dstEntity = Entity{ other->mRegistry.create(), &other->mRegistry };
                        dstEntity = other->CreateSerializedEntity(metadata, idMetadataMap);
                    }

                    CopyComponentIfExists<Ts>(srcEntity, dstEntity);
				});
			}(), ...);
        }

        template<typename... Ts>
        void CopyComponentIfExists(ComponentGroup<Ts...>, Entity from, Entity to)
        {
            CopyComponentIfExists<Ts...>(from, to);
        }

        template<typename... Ts>
        void CopyComponentIfExists(Entity from, Entity to)
        {
            ([&]()
			{
				if (from.HasComponent<Ts>())
					CopyComponent<Ts>(from, to);
			}(), ...);
        }

        template<typename... Ts>
        void CopyComponent(ComponentGroup<Ts...>, Entity from, Entity to)
        {
            CopyComponent<Ts...>(from, to);
        }

        template<typename... Ts>
        void CopyComponent(Entity from, Entity to)
        {
            ((to.AddOrReplaceComponent<Ts>(from.GetComponent<Ts>())), ...);
        }

    private:
        entt::registry mRegistry;
        EntityMetadataComponent mSceneRootMetadata;

    private:
        friend class Entity;
    };

}
