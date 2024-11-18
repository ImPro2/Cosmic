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

    void Panels::Init(const Ref<Framebuffer>& framebuffer, const Ref<Scene>& scene)
    {
        mPanels.reserve(10);

        mPanels.push_back(static_cast<Panel*>(ModuleSystem::Add<ConsolePanel>()));
        mPanels.push_back(static_cast<Panel*>(ModuleSystem::Add<SceneHierarchyPanel>(scene)));
        mPanels.push_back(static_cast<Panel*>(ModuleSystem::Add<InspectorPanel>()));
        mPanels.push_back(static_cast<Panel*>(ModuleSystem::Add<ContentBrowserPanel>()));
        mPanels.push_back(static_cast<Panel*>(ModuleSystem::Add<ViewportPanel>(framebuffer, scene, GetPanel<ContentBrowserPanel>())));
    }

    void Panels::ShowAll()
    {
        for (Panel* panel : mPanels)
        {
            panel->Show(true);
        }
    }

}
