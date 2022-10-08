module;
#include "cspch.hpp"
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <string>
module Cosmic.ECS.Scene;

CS_MODULE_LOG_INFO(Cosmic, ECS.Scene);

import Cosmic.App.Log;

namespace Cosmic
{

    Scene::Scene()
    {
    }

    Scene::~Scene()
    {

    }

    void Scene::OnUpdate(Dt dt)
    {
        // Update scripts
        {
            mRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
            {
                if (!nsc.Instance)
                {
                    nsc.Instance          = nsc.InstantiateScript();
                    nsc.Instance->mEntity = Entity{ entity, &mRegistry };
                    nsc.Instance->OnCreate();
                }
                nsc.Instance->OnUpdate(Time::GetDeltaTime());
            });
        }

        Camera*    mainCamera          = nullptr;
        glm::mat4* mainCameraTransform = nullptr;

        // Find main camera
        {
            auto view = mRegistry.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.Primary)
                {
                    mainCamera          = &camera.Camera;
                    mainCameraTransform = &transform.Transform;
                }
            }
        }

        // Only render if main camera exists
        if (mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, *mainCameraTransform);

            auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::RenderQuad(transform, sprite.Color);
            }

            Renderer2D::EndScene();
        }
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
        int index = 0;
        auto view = mRegistry.view<TagComponent>();
        std::string tag = name.empty() ? "Entity" : name;
        for (auto entity : view)
        {
            auto& otherTag = view.get<TagComponent>(entity).Tag;

            if (tag == otherTag)
            {
                tag = std::format("{}{}", tag, index);
            }

            index++;
        }

        Entity entity = { mRegistry.create(), &mRegistry };
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<TagComponent>(tag);

        return entity;
    }

    void Scene::RemoveEntity(Entity entity)
    {
        mRegistry.destroy((entt::entity)entity);
    }

    void Scene::DuplicateEntity(Entity entity)
    {
        Entity newEntity = CreateEntity(entity.GetComponent<TagComponent>().Tag);

        // TODO
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

}