module;
#include "cspch.hpp"
#include <entt/entt.hpp>
export module Cosmic.ECS.Scene;

import Cosmic.Time.DeltaTime;
import Cosmic.Time;
import Cosmic.Renderer.Renderer2D;
import Cosmic.Renderer.Camera;
import Cosmic.Base;
import Cosmic.ECS.SceneCamera;
import Cosmic.ECS.Entity;
import Cosmic.Script.NativeScript;
import Cosmic.Renderer.OrthographicCamera;

namespace Cosmic
{

    export class Scene
    {
    public:
        Scene();
        ~Scene();

    public:
        void OnUpdate(Dt dt);
        void OnUpdateEditor(Dt dt, const OrthographicCamera& camera);
        void OnViewportResize(uint32 width, uint32 height);

    public:
        Entity CreateEntity(const String& name = "");
        void   RemoveEntity(Entity entity);
        void   DuplicateEntity(Entity entity);
        Entity FindEntityByTag(const String& tag);

        void ForEachEntity(std::function<void(Entity)> fn);

    private:
        entt::registry mRegistry;

    private:
        friend class Entity;
    };

}