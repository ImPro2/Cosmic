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
			auto view = registry.view<EntityMetadataComponent>();
			String newTag = tag.empty() ? "Entity" : tag;

			for (auto entity : view)
			{
				auto& otherTag = view.get<EntityMetadataComponent>(entity).Tag;

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
        // Resolve each root entity's relative child properties

        ResolveRelativeChildProperties();

        // Update scripts

        NativeScriptEngine::OnUpdate(dt);

        // Render SpriteRendererComponents

        Renderer2D::BeginScene(camera, cameraTransform);

        mRegistry.view<EntityMetadataComponent, TransformComponent, SpriteRendererComponent>().each([](entt::entity entity, auto& metadata, auto& tc, auto& sprite)
		{
			if (metadata.IsVisible)
			{
                Renderer2D::RenderQuad(tc.GetAbsoluteTransform(), sprite.Color, (int32)entity);
            }
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

    Entity Scene::CreateEntity(const String& name, Entity parent)
    {
        EntityMetadataComponent metadata;
        metadata.ID            = Random<int32>(0, std::numeric_limits<int32>::max());
        metadata.Tag           = Utils::NewEntityNameFromTag(mRegistry, name);
        metadata.IsVisible     = true;
        metadata.ChildrenCount = 0;
        metadata.Parent        = parent;

        Entity entity = { mRegistry.create(), &mRegistry };
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<EntityMetadataComponent>(metadata);

        EntityMetadataComponent& parentMetadata = parent ? parent.GetComponent<EntityMetadataComponent>() : mSceneRootMetadata;
        RegisterEntity(entity, parentMetadata);

#if 0
		parentMetadata.ChildrenCount++;

        if (!parentMetadata.FirstChild && !parentMetadata.LastChild)
        {
            parentMetadata.FirstChild = entity;
            parentMetadata.LastChild  = entity;
        }
        else
        {
            Entity lastChild = parentMetadata.LastChild;
            lastChild.GetComponent<EntityMetadataComponent>().Next = entity;
            metadata.Prev = lastChild;

            parentMetadata.LastChild = entity;
        }
#endif

        return entity;
    }

    Entity Scene::CreateSerializedEntity(const EntityMetadataComponent& metadata)
    {
        Entity entity = { mRegistry.create(), &mRegistry };
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<EntityMetadataComponent>(metadata);

        return entity;
    }

    Entity Scene::AddEntity(Entity entity)
    {
        Entity newEntity = { mRegistry.create(), &mRegistry };

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
        ForEachChildRecurseTopDown(entity, [this](Entity child)
		{
			mRegistry.destroy((entt::entity)child);
		});

        mRegistry.destroy(entity);
    }

    void Scene::RegisterEntity(Entity entity, EntityMetadataComponent& parentMetadata)
    {
		parentMetadata.ChildrenCount++;

        if (!parentMetadata.FirstChild && !parentMetadata.LastChild)
        {
            parentMetadata.FirstChild = entity;
            parentMetadata.LastChild  = entity;
        }
        else
        {
            Entity lastChild = parentMetadata.LastChild;
            lastChild.GetComponent<EntityMetadataComponent>().Next = entity;
            entity.GetComponent<EntityMetadataComponent>().Prev = lastChild;

            parentMetadata.LastChild = entity;
        }
    }

    void Scene::RegisterSerializedEntity(Entity entity)
    {
        // Only register root entities

        if (entity.GetComponent<EntityMetadataComponent>().Parent)
            return;

		mSceneRootMetadata.ChildrenCount++;

        if (!mSceneRootMetadata.FirstChild && !mSceneRootMetadata.LastChild)
        {
            mSceneRootMetadata.FirstChild = entity;
            mSceneRootMetadata.LastChild  = entity;
        }
        else
        {
            mSceneRootMetadata.LastChild = entity;
        }
    }

    Entity Scene::FindEntityByTag(const String& tag)
    {
        if (tag == "")
			return Entity { entt::null, &mRegistry };

        for (entt::entity entity : mRegistry.view<EntityMetadataComponent>())
        {
            if (mRegistry.get<EntityMetadataComponent>(entity).Tag == tag)
                return Entity { entity, &mRegistry };
        }

        return Entity { entt::null, &mRegistry };
    }

    Entity Scene::FindEntityByID(int32 id)
    {
        if (id == -1)
			return Entity { entt::null, &mRegistry };

        for (entt::entity entity : mRegistry.view<EntityMetadataComponent>())
        {
            if (mRegistry.get<EntityMetadataComponent>(entity).ID == id)
                return Entity { entity, &mRegistry };
        }

        return Entity { entt::null, &mRegistry };
    }

    Entity Scene::FindRootParent(Entity entity)
    {
        Entity parent = entity.GetComponent<EntityMetadataComponent>().Parent;

        if (parent)
            return FindRootParent(parent);

		return entity;
    }

    void Scene::ForEachRootEntity(std::function<void(Entity)> fn)
    {
        Entity entity = mSceneRootMetadata.FirstChild;

        while (entity)
        {
            fn(entity);
            entity = entity.GetComponent<EntityMetadataComponent>().Next;
        }
    }

    void Scene::ForEachEntity(std::function<void(Entity)> fn)
    {
        auto view = mRegistry.view<EntityMetadataComponent>();
        for (auto entity : view)
            fn({ entity, &mRegistry });
    }

    void Scene::ForEachEntityIndexed(std::function<void(Entity, int32)> fn)
    {
        auto view = mRegistry.view<EntityMetadataComponent>();

        int32 i = 0;

        for (auto entity : view)
        {
            fn({ entity, &mRegistry }, i++);
        }
    }

    void Scene::ForEachChild(Entity parent, std::function<void(Entity)> fn)
    {
        ForEachChild(parent.GetComponent<EntityMetadataComponent>(), fn);
    }

    void Scene::ForEachChild(const EntityMetadataComponent& parentMetadata, std::function<void(Entity)> fn)
    {
        Entity child = parentMetadata.FirstChild;

        while (child)
        {
            fn(child);

            child = child.GetComponent<EntityMetadataComponent>().Next;
        }
    }

    void Scene::ForEachChildRecurseTopDown(Entity parent, std::function<void(Entity)> fn)
    {
        ForEachChildRecurseTopDown(parent.GetComponent<EntityMetadataComponent>(), fn);
    }

    void Scene::ForEachChildRecurseTopDown(const EntityMetadataComponent& parentMetadata, std::function<void(Entity)> fn)
    {
		Entity child = parentMetadata.FirstChild;

		while (child)
		{
            fn(child);

			auto& metadata = child.GetComponent<EntityMetadataComponent>();
			if (metadata.ChildrenCount > 0)
				ForEachChildRecurseTopDown(child, fn);

            child = metadata.Next;
		}
    }

    void Scene::ForEachChildRecurseBottomUp(Entity parent, std::function<void(Entity)> fn)
    {
        ForEachChildRecurseBottomUp(parent.GetComponent<EntityMetadataComponent>(), fn);
    }

    void Scene::ForEachChildRecurseBottomUp(const EntityMetadataComponent& parentMetadata, std::function<void(Entity)> fn)
    {
		Entity child = parentMetadata.FirstChild;

		while (child)
		{
			auto& metadata = child.GetComponent<EntityMetadataComponent>();
			if (metadata.ChildrenCount > 0)
				ForEachChildRecurseTopDown(child, fn);

            fn(child);

            child = metadata.Next;
		}
    }

    size_t Scene::GetEntityCount() const
    {
        return mRegistry.view<entt::entity>().size();
    }

    void Scene::ResolveRelativeChildProperties()
    {
        ForEachRootEntity([this](Entity entity) { ResolveRelativeChildPropertiesRecurse(entity); });
    }

    void Scene::ResolveRelativeChildPropertiesRecurse(Entity entity)
    {
        EntityMetadataComponent& metadata = entity.GetComponent<EntityMetadataComponent>();

        if (!metadata.Parent)
        {
			TransformComponent& tc = entity.GetComponent<TransformComponent>();
			tc.mAbsoluteTranslation = tc.Translation;
			tc.mAbsoluteRotation    = tc.Rotation;
			tc.mAbsoluteScale       = tc.Scale;
        }
        else
        {
            // EntityMetadataComponent

			const EntityMetadataComponent& parentMetadata = metadata.Parent.GetComponent<EntityMetadataComponent>();

			if (!parentMetadata.IsVisible)
				metadata.IsVisible = false;

            // TransformComponent

			const TransformComponent& parentTransform = metadata.Parent.GetComponent<TransformComponent>();
			TransformComponent& tc = entity.GetComponent<TransformComponent>();

			if (tc.IsRelative)
			{
				tc.mAbsoluteTranslation = tc.Translation + parentTransform.mAbsoluteTranslation;
				tc.mAbsoluteRotation    = tc.Rotation    + parentTransform.mAbsoluteRotation;
				tc.mAbsoluteScale       = tc.Scale       * parentTransform.mAbsoluteScale;
			}
        }

        ForEachChildRecurseTopDown(entity, [this](Entity child) { ResolveRelativeChildPropertiesRecurse(child); });
    }

}
