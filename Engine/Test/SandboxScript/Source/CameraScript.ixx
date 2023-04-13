module;
#include "cspch.hpp"
#include <glm/glm.hpp>
#include <entt/entt.hpp>
export module SandboxScript.CameraScript;

CS_MODULE_LOG_INFO(SandboxScript, CameraScript);

import Cosmic.App.Application;
import Cosmic.Script.NativeScript;
import Cosmic.Base;
import Cosmic.App.Log;
import Cosmic.App.Input;
import Cosmic.ECS.Components;
import Cosmic.Time.DeltaTime;
import Cosmic.App.Application;

namespace Cosmic
{

    export class CameraScript : public NativeScript
    {
    public:
        void OnCreate() override
        {
            CS_LOG_INFO("Hello, world!");
        }

        void OnUpdate(Dt dt) override
        {
            Application* app = Application::Get();
            auto& registry = mEntity.GetRegistry();

            auto& transform = GetComponent<TransformComponent>().Transform;

            glm::vec2 direction = {
                (float32)Input::IsKeyPressed(EKeyCode::A) - (float32)Input::IsKeyPressed(EKeyCode::A),
                (float32)Input::IsKeyPressed(EKeyCode::W) - (float32)Input::IsKeyPressed(EKeyCode::S)
            };
            
            transform[3][0] += mAcceleration * direction.x * dt;
            transform[3][1] += mAcceleration * direction.y * dt;
        }

    private:
        float32 mAcceleration = 5.0f;
    };

}
