#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <entt/entt.hpp>

#include "Base/Base.hpp"
#include "ECS/SceneCamera.hpp"
#include "Script/NativeScript.hpp"
#include "Script/ScriptEngine.hpp"
#include "Time/DeltaTime.hpp"
#include "Time/Time.hpp"

namespace Cosmic
{

    struct TagComponent
    {
        String Tag = "";

        TagComponent()                    = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const String& tag)
            : Tag(tag)
        {
        }

        void Reset()
        {
            Tag = "";
        }

        operator String& ()             { return Tag; }
        operator const String& () const { return Tag; }
    };

    struct TransformComponent
    {
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation    = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale       = { 1.0f, 1.0f, 1.0f };

        TransformComponent()                          = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation)
            : Translation(translation)
        {
        }

        glm::mat4 GetTransform() const
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

            return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
        }

        void Reset()
        {
            Translation = { 0.0f, 0.0f, 0.0f };
            Rotation    = { 0.0f, 0.0f, 0.0f };
            Scale       = { 1.0f, 1.0f, 1.0f };
        }
    };

    struct SpriteRendererComponent
    {
        float4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        //xRef<Texture2D> Texture;

        SpriteRendererComponent()                               = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(float4 color)
            : Color(color)
        {
        }

        void Reset()
        {
            Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        }

        operator float4& ()             { return Color; }
        operator const float4& () const { return Color; }
    };

    struct CameraComponent
    {
        SceneCamera Camera;
        bool        Primary = true;
        bool        FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        void Reset()
        {
            Primary = true;
            FixedAspectRatio = false;
        }
    };

    struct NativeScriptComponent
    {
        NativeScript*         Instance        = nullptr;
        NativeScriptCallbacks ScriptCallbacks = {};
        String                ClassName       = "";
        bool                  Bound           = false;

        void Bind(const String& className)
        {
            ScriptCallbacks = ScriptEngine::AddNativeScript(className);
            Bound = true;
            ClassName = className;
        }

        void Reset()
        {
            Instance = nullptr;
            
            ScriptCallbacks.InstantiateScript = []() -> NativeScript* { return nullptr; };
            ScriptCallbacks.DestroyScript     = [](NativeScript* instance) { };

            ClassName = "";
            Bound = false;
        }
    };

}
