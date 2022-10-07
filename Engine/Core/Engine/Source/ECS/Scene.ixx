module;
#include "cspch.hpp"
#include <entt/entt.hpp>
export module Cosmic.ECS.Scene;

import Cosmic.Time.DeltaTime;
import Cosmic.Renderer.Renderer2D;
import Cosmic.Base;
import Cosmic.ECS.Components;
import Cosmic.ECS.Entity;

namespace Cosmic
{

    export class Scene
    {
    public:
        Scene();
        ~Scene();

    public:
        void OnUpdate(Dt dt);

    public:
        Entity CreateEntity(const String& name = "");
        void   RemoveEntity(Entity entity);
        void   DuplicateEntity(Entity entity);
        Entity FindEntityByTag(const String& tag);

        void ForEachEntity(std::function<void(Entity)> fn);

    private:
        entt::registry mRegistry;
    };

}