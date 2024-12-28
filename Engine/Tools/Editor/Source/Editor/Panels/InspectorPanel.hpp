#pragma once
#include <entt/entt.hpp>

#include "Panels.hpp"
#include "SceneHierarchyPanel.hpp"
#include "PlaybarPanel.hpp"

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
        void OnInit() override;
        void OnImGuiRender() override;

    private:
        void RenderComponents(Entity entity);

    private:
        Ref<SceneHierarchyPanel> mSceneHierarchyPanel;
        Ref<PlaybarPanel>        mPlaybarPanel;
    };

}