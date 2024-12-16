#include "cspch.hpp"
#include "PlayerScript.hpp"
#include "Memory/DefaultAllocator.hpp"
#include "App/Application.hpp"

CS_MODULE_LOG_INFO(SandboxProject, PlayerScript);

namespace Cosmic
{

    PlayerScript::PlayerScript(Entity entity)
        : NativeScript(entity)
    {
    }

    void PlayerScript::OnInstantiate()
    {
        CS_LOG_INFO("OnInstantiate");
    }

    void PlayerScript::OnUpdate(Dt dt)
    {
        CS_LOG_INFO("OnUpdate: {}ms", Time::GetFPS().InSeconds());
    }

}

CS_NATIVE_SCRIPT(PlayerScript);
