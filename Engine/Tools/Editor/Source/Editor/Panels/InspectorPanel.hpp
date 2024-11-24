#pragma once
#include <entt/entt.hpp>

#include "Panels.hpp"
#include "SceneHierarchyPanel.hpp"

#include "Base/Base.hpp"
#include "App/Log/Log.hpp"
#include "App/Module.hpp"
#include "ECS/Components.hpp"
#include "ECS/Scene.hpp"
#include "ECS/Entity.hpp"

namespace Cosmic
{

    class InspectorPanel : public IPanel
    {
    public:
        InspectorPanel();

    public:
        void OnImGuiRender() override;

    private:
        void RenderComponents(Entity entity);
    };

}