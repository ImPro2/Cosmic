#include "cspch.hpp"
#include "Panels.hpp"
#include <entt/entt.hpp>

#include "ViewportPanel.hpp"
#include "ConsolePanel.hpp"
#include "SceneHierarchyPanel.hpp"
#include "InspectorPanel.hpp"

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