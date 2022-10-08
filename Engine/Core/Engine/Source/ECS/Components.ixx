module;
#include "cspch.hpp"
#include <glm/glm.hpp>
#include <entt/entt.hpp>
export module Cosmic.ECS.Components;

import Cosmic.Base;
import Cosmic.ECS.SceneCamera;
import Cosmic.ECS.NativeScript;
import Cosmic.Time.DeltaTime;
import Cosmic.Time;

namespace Cosmic
{

    export struct TagComponent
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

    export struct TransformComponent
    {
        glm::mat4 Transform = glm::mat4(1.0f);

        TransformComponent()                          = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform)
            : Transform(transform)
        {
        }

        void Reset()
        {
            Transform = glm::mat4(1.0f);
        }

        operator glm::mat4& ()             { return Transform; }
        operator const glm::mat4& () const { return Transform; }
    };

    export struct SpriteRendererComponent
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

    export struct CameraComponent
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

    export struct NativeScriptComponent
    {
        NativeScript* Instance = nullptr;

        NativeScript* (*InstantiateScript)();
        void (*DestroyScript)(NativeScriptComponent*);

        template<typename T>
        void Bind()
        {
            InstantiateScript = []()          -> NativeScript* { return new T();                               };
            DestroyScript     = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
        }

        void Reset()
        {
            Instance          = nullptr;
 
            InstantiateScript = []() -> NativeScript*          { return nullptr; };
            DestroyScript     = [](NativeScriptComponent* nsc) {                 };
        }
    };

}