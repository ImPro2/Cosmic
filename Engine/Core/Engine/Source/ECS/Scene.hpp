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
        template<size_t I = 0, typename... Ts, typename F>
        constexpr void IterateComponentsInTuple(std::tuple<Ts...> tup, F callback)
        {
            if constexpr (I != sizeof...(Ts))
            {
                callback(static_cast<IComponent*>(&std::get<I>(tup)));
                IterateComponentsInTuple<I + 1>(tup, callback);
            }
        }

        template<typename... Ts, typename F>
        void ResolveRelativeChildPropertiesRecurse(Entity entity, F callback, const Ts&... components)
        {
            std::function<void(Entity, F, std::tuple<Ts...>)> recurseFn;

            recurseFn = [this, &recurseFn](Entity entity, F callback, std::tuple<Ts...> parentComponents)
			{
				std::tuple<Ts...> components;

                IterateComponentsInTuple<0, Ts...>(parentComponents, [&components, &entity](IComponent* parentComponent)
				{
					switch (parentComponent->GetType())
					{
						case EComponentType::EntityMetadata:
						{
                            const EntityMetadataComponent& component = *static_cast<EntityMetadataComponent*>(parentComponent);
							EntityMetadataComponent& metadata = entity.GetComponent<EntityMetadataComponent>();

							if (!component.IsVisible)
								metadata.IsVisible = false;

							std::get<EntityMetadataComponent>(components) = metadata;                           
							break;
						}
						case EComponentType::Transform:
						{
                            const TransformComponent& component = *static_cast<TransformComponent*>(parentComponent);
							TransformComponent tc = entity.CopyComponent<TransformComponent>();

							if (tc.IsRelative)
							{
								tc.Translation += component.Translation;
								tc.Rotation    += component.Rotation;
								tc.Scale       *= component.Scale;
							}

							std::get<TransformComponent>(components) = tc;
							break;
						}
                    }
				});

                std::apply([entity, &callback](const Ts&... components) { callback(entity, components...); }, components);

                Entity child = entity.GetComponent<EntityMetadataComponent>().FirstChild;

                while (child)
                {
                    recurseFn(child, callback, components);
                    child = child.GetComponent<EntityMetadataComponent>().Next;
                }
			};

            recurseFn(entity, callback, std::make_tuple(components...));
        }

    private:
        entt::registry mRegistry;
        EntityMetadataComponent mSceneRootMetadata;

    private:
        friend class Entity;
    };

}
