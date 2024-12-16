#include "cspch.hpp"
#include "PlayerScript.hpp"

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
    
    Ref<NativeScript> CSInstantiatePlayerScript(Entity entity)
    {
        return CreateRef<PlayerScript>(entity);
    }

}