module;
#include "cspch.hpp"
module Editor.Panels;

import Editor.Panels.ViewportPanel;

namespace Cosmic
{

    void Panels::Init(const Ref<Framebuffer>& framebuffer)
    {
        mPanels.reserve(10);

        ModuleSystem::Add<ViewportPanel>(framebuffer);
        mPanels.push_back(static_cast<Panel*>(ModuleSystem::Get<ViewportPanel>()));
    }

    void Panels::ShowAll()
    {
        for (Panel* panel : mPanels)
        {
            panel->Show(true);
        }
    }

}