module;
#include "cspch.hpp"
#include <entt/entt.hpp>
module Editor.Panels;

import Editor.Panels.ViewportPanel;
import Editor.Panels.ConsolePanel;
import Editor.Panels.SceneHierarchyPanel;
import Editor.Panels.InspectorPanel;

namespace Cosmic
{

    void Panels::Init(const Ref<Framebuffer>& framebuffer, const Ref<Scene>& scene)
    {
        mPanels.reserve(10);

        ModuleSystem::Add<ViewportPanel>(framebuffer, scene);
        mPanels.push_back(static_cast<Panel*>(ModuleSystem::Get<ViewportPanel>()));
 
        ModuleSystem::Add<ConsolePanel>();
        mPanels.push_back(static_cast<Panel*>(ModuleSystem::Get<ConsolePanel>()));
        
        ModuleSystem::Add<SceneHierarchyPanel>(scene);
        mPanels.push_back(static_cast<Panel*>(ModuleSystem::Get<SceneHierarchyPanel>()));

        ModuleSystem::Add<InspectorPanel>();
        mPanels.push_back(static_cast<Panel*>(ModuleSystem::Get<InspectorPanel>()));
    }

    void Panels::ShowAll()
    {
        for (Panel* panel : mPanels)
        {
            panel->Show(true);
        }
    }

}