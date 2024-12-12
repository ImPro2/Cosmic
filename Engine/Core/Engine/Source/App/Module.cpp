#include "cspch.hpp"
#include "App/Module.hpp"

namespace Cosmic
{

    void ModuleSystem::Init()
    {
        CS_PROFILE_FN();
    }

    void ModuleSystem::Shutdown()
    {
        CS_PROFILE_FN();

        sModules.clear();
    }

    void ModuleSystem::OnUpdate()
    {
        CS_PROFILE_FN();

        AddDeferredModules();
        RemoveDeferredModules();

        float32 dt = Time::GetDeltaTime();

        for (Ref<IModule> module : sModules)
            module->OnUpdate(dt);
    }

    void ModuleSystem::OnEvent(const IEvent& e)
    {
        CS_PROFILE_FN();

        for (Ref<IModule> module : sModules)
            module->OnEvent(e);
    }

    void ModuleSystem::OnImGuiRender()
    {
        CS_PROFILE_FN();

        for (Ref<IModule> module : sModules)
            module->OnImGuiRender();
    }

    void ModuleSystem::AddDeferredModules()
    {
        while (!sDeferredAddModules.empty())
        {
            auto [module, insertMode] = sDeferredAddModules.front();
            
            module->OnInit();
            
            switch (insertMode)
            {
                case EDeferredInsertMode::Front: sModules.insert(sModules.begin(), module); break;
                case EDeferredInsertMode::Back:  sModules.push_back(module);                break;
            }

            sDeferredAddModules.pop();
        }
    }

    void ModuleSystem::RemoveDeferredModules()
    {
        while (!sDeferredRemoveModules.empty())
        {
            Ref<IModule> module = sDeferredRemoveModules.front();

            std::erase(sModules, module);
            module->OnShutdown();
            module.Release();

            sDeferredRemoveModules.pop();
        }
    }

}
