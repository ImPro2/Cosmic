#include "cspch.hpp"
#include "ECS/Scene.hpp"
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <string>

CS_MODULE_LOG_INFO(Cosmic, ECS.Scene);

#include "App/Log/Log.hpp"
#include "ECS/Components.hpp"
#include "Base/Random.hpp"
#include "Script/NativeScriptEngine.hpp"

namespace Cosmic
{

    namespace Utils
    {

        String NewEntityNameFromTag(const entt::registry& registry, const String& tag)
        {
			int index = 0;
			auto view = registry.view<TagComponent>();
			String newTag = tag.empty() ? "Entity" : tag;

			for (auto entity : view)
			{
				auto& otherTag = view.get<TagComponent>(entity).Tag;

				if (newTag == otherTag)
				{
					newTag = std::format("{} {}", tag, index);
                    break;
				}

				index++;
			}

            return newTag;
        }

    }

    Scene::Scene()
    {
    }

    Scene::~Scene()
    {

    }

    void Scene::OnUpdate(Dt dt)
    {
        // Update scripts

        NativeScriptEngine::OnUpdate(dt);

        Camera*   mainCamera          = nullptr;
        glm::mat4 mainCameraTransform = glm::mat4(1.0f);

        // Find main camera
        {
            auto view = mRegistry.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.Primary)
                {
                    mainCamera          = &camera.Camera;
                    mainCameraTransform = transform.GetTransform();
                }
            }
        }

        // Only render if main camera exists
        if (mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, mainCameraTransform);

            /*auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::RenderQuad(transform.GetTransform(), sprite.Color);
            }*/

            mRegistry.view<EntityMetadataComponent, TransformComponent, SpriteRendererComponent>().each([](auto entity, auto& metadata, auto& transform, auto& sprite)
            {
                if (metadata.IsVisible)
                    Renderer2D::RenderQuad(transform.GetTransform(), sprite.Color);
            });

            Renderer2D::EndScene();
        }
    }

    void Scene::OnUpdateEditor(Dt dt, const Camera& camera, const glm::mat4& cameraTransform)
    {
        // Update scripts

        NativeScriptEngine::OnUpdate(dt);

        // Render SpriteRendererComponents

        Renderer2D::BeginScene(camera, cameraTransform);

        mRegistry.view<EntityMetadataComponent, TransformComponent, SpriteRendererComponent>().each([](auto entity, auto& metadata, auto& transform, auto& sprite)
        {
            if (metadata.IsVisible)
                Renderer2D::RenderQuad(transform.GetTransform(), sprite.Color, (int32)entity);
                //Renderer2D::RenderQuad(transform.GetTransform(), sprite.Color, (int)entity);
        });

        Renderer2D::EndScene();
    }

    void Scene::OnViewportResize(uint32 width, uint32 height)
    {
        // Resize non-fixed aspect ratio cameras.

        auto view = mRegistry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
            {
                cameraComponent.Camera.SetViewportSize(width, height);
            }
        }
    }

    Entity Scene::CreateEntity(const String& name)
    {
        EntityMetadataComponent metadata;
        metadata.ID        = Random<int32>(0, std::numeric_limits<int32>::max());
        metadata.IsVisible = true;

        Entity entity = { mRegistry.create(), &mRegistry };
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<TagComponent>(Utils::NewEntityNameFromTag(mRegistry, name));
        entity.AddComponent<EntityMetadataComponent>(metadata);

        return entity;
    }

    Entity Scene::AddEntity(Entity entity)
    {
        Entity newEntity = { mRegistry.create(), &mRegistry };

		newEntity.AddComponent<TagComponent>(Utils::NewEntityNameFromTag(mRegistry, entity.GetComponent<TagComponent>().Tag));
		newEntity.AddComponent<EntityMetadataComponent>(entity.GetComponent<EntityMetadataComponent>());
		newEntity.AddComponent<TransformComponent>(entity.GetComponent<TransformComponent>());

        if (entity.HasComponent<SpriteRendererComponent>())
			newEntity.AddComponent<SpriteRendererComponent>(entity.GetComponent<SpriteRendererComponent>());

        if (entity.HasComponent<CameraComponent>())
			newEntity.AddComponent<CameraComponent>(entity.GetComponent<CameraComponent>());

        if (entity.HasComponent<NativeScriptComponent>())
			newEntity.AddComponent<NativeScriptComponent>(entity.GetComponent<NativeScriptComponent>());

        return newEntity;
    }

    void Scene::RemoveEntity(Entity entity)
    {
        int32 ID = entity.GetComponent<EntityMetadataComponent>().ID;

        for (entt::entity other : mRegistry.view<EntityMetadataComponent>())
        {
            int32 otherID = mRegistry.get<EntityMetadataComponent>(other).ID;

            if (ID == otherID)
            {
                mRegistry.destroy(other);
                break;
            }
        }
    }

    Entity Scene::FindEntityByTag(const String& tag)
    {
        auto view = mRegistry.view<TagComponent>();
        for (auto entity : view)
        {
            TagComponent& tagComponent = view.get<TagComponent>(entity);
            if (tagComponent.Tag == tag)
                return { entity, &mRegistry };
        }
    }

    void Scene::ForEachEntity(std::function<void(Entity)> fn)
    {
        auto view = mRegistry.view<TagComponent>();
        for (auto entity : view)
            fn({ entity, &mRegistry });
    }

    void Scene::ForEachEntityIndexed(std::function<void(Entity, int32)> fn)
    {
        auto view = mRegistry.view<TagComponent>();
        int32 i = 0;
        for (auto entity : view)
        {
            fn({ entity, &mRegistry }, i);
            i++;
        }
    }

    size_t Scene::GetEntityCount() const
    {
        return mRegistry.view<entt::entity>().size();
    }

}
