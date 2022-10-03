module;
#include "cspch.hpp"
module Cosmic.App.Module;

namespace Cosmic
{

    void ModuleSystem::Init()
    {
        CS_PROFILE_FN();

        //xsModules.reserve(100);
    }

    void ModuleSystem::Shutdown()
    {
        CS_PROFILE_FN();

        sModules.clear();
    }

    void ModuleSystem::OnUpdate()
    {
        CS_PROFILE_FN();

        for (Module* module : sModules)
            module->OnUpdate(float32(Time::GetDeltaTime()));
    }

    void ModuleSystem::OnEvent(const Event& e)
    {
        CS_PROFILE_FN();

        for (Module* module : sModules)
            module->OnEvent(e);
    }

    void ModuleSystem::OnImGuiRender()
    {
        CS_PROFILE_FN();

        for (Module* module : sModules)
            module->OnImGuiRender();
    }

}