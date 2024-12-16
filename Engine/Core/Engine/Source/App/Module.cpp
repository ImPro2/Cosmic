#include "cspch.hpp"
#include "App/Module.hpp"

namespace Cosmic
{

    Ref<ModuleSystem> ModuleSystem::Init()
    {
        CS_PROFILE_FN();

        sInstance = CreateRef<ModuleSystem>();

        return sInstance;
    }

    void ModuleSystem::Shutdown()
    {
        CS_PROFILE_FN();

        sInstance->mModules.clear();
        sInstance.Release();
    }

    void ModuleSystem::OnUpdate()
    {
        CS_PROFILE_FN();

        AddDeferredModules();
        RemoveDeferredModules();

        float32 dt = Time::GetDeltaTime();

        for (Ref<IModule> module : sInstance->mModules)
            module->OnUpdate(dt);
    }

    void ModuleSystem::OnEvent(const IEvent& e)
    {
        CS_PROFILE_FN();

        for (Ref<IModule> module : sInstance->mModules)
            module->OnEvent(e);
    }

    void ModuleSystem::OnImGuiRender()
    {
        CS_PROFILE_FN();

        for (Ref<IModule> module : sInstance->mModules)
            module->OnImGuiRender();
    }

    void ModuleSystem::AddDeferredModules()
    {
        while (!sInstance->mDeferredAddModules.empty())
        {
            auto [module, insertMode] = sInstance->mDeferredAddModules.front();
            
            module->OnInit();
            
            switch (insertMode)
            {
                case EDeferredInsertMode::Front: sInstance->mModules.insert(sInstance->mModules.begin(), module); break;
                case EDeferredInsertMode::Back:  sInstance->mModules.push_back(module);                           break;
            }

            sInstance->mDeferredAddModules.pop();
        }
    }

    void ModuleSystem::RemoveDeferredModules()
    {
        while (!sInstance->mDeferredRemoveModules.empty())
        {
            Ref<IModule> module = sInstance->mDeferredRemoveModules.front();

            std::erase(sInstance->mModules, module);
            module->OnShutdown();
            module.Release();

            sInstance->mDeferredRemoveModules.pop();
        }
    }

}
