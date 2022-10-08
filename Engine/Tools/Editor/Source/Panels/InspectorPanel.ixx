module;
#include "cspch.hpp"
#include <entt/entt.hpp>
export module Editor.Panels.InspectorPanel;

import Cosmic.Base;
import Cosmic.App.Log;
import Cosmic.App.Module;
import Cosmic.ECS.Components;
import Cosmic.ECS.Scene;
import Cosmic.ECS.Entity;

import Editor.Panels;
import Editor.Panels.SceneHierarchyPanel;

namespace Cosmic
{

    export class InspectorPanel : public Panel
    {
    public:
        InspectorPanel();

    public:
        void OnImGuiRender() override;

    private:
        void RenderComponents(Entity entity);
    };

}