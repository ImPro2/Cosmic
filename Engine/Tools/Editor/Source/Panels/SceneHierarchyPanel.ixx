module;
#include "cspch.hpp"
#include <imgui.h>
#include <entt/entt.hpp>
export module Editor.Panels.SceneHierarchyPanel;

import Editor.Panels;

import Cosmic.Base;
import Cosmic.App.Module;
import Cosmic.App.Log;
import Cosmic.App.Input;
import Cosmic.App.KeyAndMouseCodes;
import Cosmic.Time.DeltaTime;
import Cosmic.ECS.Scene;
import Cosmic.ECS.Entity;
import Cosmic.ECS.Components;
import Cosmic.App.Events;
import Cosmic.App.WindowEvents;

namespace Cosmic
{

    export class SceneHierarchyPanel : public Panel
    {
    public:
        SceneHierarchyPanel(const Ref<Scene>& scene);

    public:
        void OnEvent(const Event& e) override;
        bool OnMouseButtonClick(const MouseButtonClickEvent& e);
        void OnImGuiRender() override;

    public:
        Entity GetSelectedEntity() { return mSelectedEntity; }

    private:
        Ref<Scene> mScene;
        Entity mSelectedEntity;
        bool mClicked = false;
    };

}