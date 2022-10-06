module;
#include "cspch.hpp"
module Editor.Panels;

import Editor.Panels.ViewportPanel;
import Editor.Panels.ConsolePanel;

namespace Cosmic
{

    void Panels::Init(const Ref<Framebuffer>& framebuffer)
    {
        mPanels.reserve(10);

        ModuleSystem::Add<ViewportPanel>(framebuffer);
        mPanels.push_back(static_cast<Panel*>(ModuleSystem::Get<ViewportPanel>()));
        ModuleSystem::Add<ConsolePanel>();
        mPanels.push_back(static_cast<Panel*>(ModuleSystem::Get<ConsolePanel>()));
    }

    void Panels::ShowAll()
    {
        for (Panel* panel : mPanels)
        {
            panel->Show(true);
        }
    }

}