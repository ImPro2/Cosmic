module;
#include "cspch.hpp"
#include <glm/glm.hpp>
export module Cosmic.ECS.Components;

import Cosmic.Base;

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

        operator float4& ()             { return Color; }
        operator const float4& () const { return Color; }
    };

}