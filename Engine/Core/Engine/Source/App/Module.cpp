module;
#include "cspch.hpp"
module Cosmic.App.Module;

namespace Cosmic
{

    void ModuleSystem::Init()
    {
        sModules.reserve(100);
    }

    void ModuleSystem::Shutdown()
    {
        sModules.clear();
    }

    void ModuleSystem::OnUpdate()
    {
        for (Module* module : sModules)
            module->OnUpdate();
    }

    void ModuleSystem::OnEvent(const WindowEvent& e)
    {
        for (Module* module : sModules)
            module->OnEvent(e);
    }

}