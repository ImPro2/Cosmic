#include "cspch.hpp"
#include "Panels.hpp"
#include <entt/entt.hpp>

#include "ViewportPanel.hpp"
#include "ConsolePanel.hpp"
#include "SceneHierarchyPanel.hpp"
#include "InspectorPanel.hpp"
#include "ContentBrowserPanel.hpp"

namespace Cosmic
{

    void Panels::Init()
    {
        mPanels.reserve(10);

        mPanels.push_back(ModuleSystem::Add<ConsolePanel>());
        mPanels.push_back(ModuleSystem::Add<SceneHierarchyPanel>());
        mPanels.push_back(ModuleSystem::Add<InspectorPanel>());
        mPanels.push_back(ModuleSystem::Add<ContentBrowserPanel>());
        mPanels.push_back(ModuleSystem::Add<ViewportPanel>());
    }

    void Panels::ShowAll()
    {
        for (Panel* panel : mPanels)
        {
            panel->Show(true);
        }
    }

}
