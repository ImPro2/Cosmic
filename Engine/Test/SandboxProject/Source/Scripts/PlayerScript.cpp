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

        bool isGrounded = tc.Translation.y <= mFloorBoundary;

        if (isGrounded)
        {
            mVelocity->x = 0.0f;
            mVelocity->y = Input::IsKeyPressed(EKeyCode::Space) ? (float32)mJumpStrength : 0.0f;
        }
        else
            mVelocity->y -= mGravity * dt;

        if (Input::IsKeyPressed(EKeyCode::A))
            mVelocity->x = -mSpeed;
        if (Input::IsKeyPressed(EKeyCode::D))
            mVelocity->x = mSpeed;

        tc.Translation += dt.InSeconds() * glm::vec3(mVelocity->x, mVelocity->y, mVelocity->z);
    }

}

