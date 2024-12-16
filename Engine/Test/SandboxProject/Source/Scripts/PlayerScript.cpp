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
        Application* instance = Application::Get();
        CS_LOG_INFO("hi");
    }

}

CS_NATIVE_SCRIPT(PlayerScript);
