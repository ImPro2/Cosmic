#include "cspch.hpp"
#include "PlayerScript.hpp"
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
        auto& tc = GetComponent<TransformComponent>();

        float3 acceleration = { 0.0f, -mGravity, 0.0f };

        if (Input::IsKeyPressed(EKeyCode::Space))
            acceleration.y = 1.0f;

        mVelocity      += dt.InSeconds() * acceleration * mMovementMultiplier;
        tc.Translation += dt.InSeconds() * glm::vec3(mVelocity->x, mVelocity->y, mVelocity->z);
    }

}

