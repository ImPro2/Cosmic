#include "cspch.hpp"
#include "PlayerScript.hpp"
#include "Memory/DefaultAllocator.hpp"
#include "App/Application.hpp"
#include "ECS/Components.hpp"
#include "App/Input.hpp"

CS_MODULE_LOG_INFO(SandboxProject, PlayerScript);

namespace Cosmic
{

    PlayerScript::PlayerScript(Entity entity)
        : NativeScript(entity)
    {
    }

    void PlayerScript::OnInstantiate()
    {
    }

    void PlayerScript::OnUpdate(Dt dt)
    {
        CS_LOG_INFO("aaa");

        auto& tc = GetComponent<TransformComponent>();

        glm::vec3 movementDir = {
            (float32)Input::IsKeyPressed(EKeyCode::A) - (float32)Input::IsKeyPressed(EKeyCode::D),
            (float32)Input::IsKeyPressed(EKeyCode::S) - (float32)Input::IsKeyPressed(EKeyCode::W),
            0.0f
        };

        glm::normalize(movementDir);

        tc.Translation += movementDir;
    }

}

CS_NATIVE_SCRIPT(PlayerScript);
