#pragma once
#include <entt/entt.hpp>

#include "Time/DeltaTime.hpp"
#include "Time/Time.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Renderer/Camera.hpp" 
#include "Base/Base.hpp"
#include "ECS/SceneCamera.hpp"
#include "ECS/Entity.hpp"
#include "Script/NativeScript.hpp"
#include "Renderer/OrthographicCamera.hpp"
#include "ECS/SceneCamera.hpp"
#include "entt/entity/fwd.hpp"

namespace Cosmic
{

    class Scene
    {
    public:
        Scene();
        ~Scene();

    public:
        void OnUpdate(Dt dt);
        void OnUpdateEditor(Dt dt, const Camera& camera, const glm::mat4& cameraTransform);
        void OnViewportResize(uint32 width, uint32 height);

    public:
        Entity CreateEntity(const String& name = "");
        void   RemoveEntity(Entity entity);
        Entity FindEntityByTag(const String& tag);

        void ForEachEntity(std::function<void(Entity)> fn);
        void ForEachEntityIndexed(std::function<void(Entity, int32)> fn);

        size_t GetEntityCount() const;
        entt::registry* GetRegistryPtr() { return &mRegistry; }

    private:
        entt::registry mRegistry;

    private:
        friend class Entity;
    };

}
