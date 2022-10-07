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
        // Render Quads

        auto view = mRegistry.view<TransformComponent, SpriteRendererComponent>();
        for (auto entity : view)
        {
            auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::RenderQuad(transform, sprite.Color);
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