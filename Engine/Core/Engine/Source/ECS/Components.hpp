#pragma once
#include "ECS/Entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <entt/entt.hpp>

#include "Base/Base.hpp"
#include "ECS/SceneCamera.hpp"
#include "Script/NativeScript.hpp"
#include "Script/NativeScriptEngine.hpp"
#include "Time/DeltaTime.hpp"
#include "Time/Time.hpp"

#define CS_COMPONENT_TYPE(type)                                      \
	EComponentType        GetType() const override { return type; }  \
	static EComponentType GetStaticType()          { return type; }


namespace Cosmic
{

    enum class EComponentType
    {
        Tag, EntityMetadata, Transform, SpriteRenderer, Camera, NativeScript
    };

    struct IComponent
    {
        virtual void           Reset()         = 0;
        virtual EComponentType GetType() const = 0;
    };

    struct TagComponent : public IComponent
    {
        String Tag = "";

        TagComponent()                    = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const String& tag)
            : Tag(tag)
        {
        }

        void Reset() override
        {
            Tag = "";
        }

        operator String& ()             { return Tag; }
        operator const String& () const { return Tag; }

        CS_COMPONENT_TYPE(EComponentType::Tag);
    };

    struct EntityMetadataComponent : public IComponent
    {
        int32 ID        = -1;
        bool  IsVisible = true;
    
        EntityMetadataComponent() = default;
        EntityMetadataComponent(const EntityMetadataComponent&) = default;
        EntityMetadataComponent(int32 id, bool isVisible)
            : ID(id), IsVisible(isVisible)
        {
        }

        void Reset() override
        {
            ID        = -1;
            IsVisible = true;
        }

        CS_COMPONENT_TYPE(EComponentType::EntityMetadata);
    };

    struct TransformComponent : public IComponent
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

        void Reset() override
        {
            Translation = { 0.0f, 0.0f, 0.0f };
            Rotation    = { 0.0f, 0.0f, 0.0f };
            Scale       = { 1.0f, 1.0f, 1.0f };
        }

        CS_COMPONENT_TYPE(EComponentType::Transform);
    };

    struct SpriteRendererComponent : public IComponent
    {
        float4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        //xRef<Texture2D> Texture;

        SpriteRendererComponent()                               = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(float4 color)
            : Color(color)
        {
        }

        void Reset() override
        {
            Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        }

        operator float4& ()             { return Color; }
        operator const float4& () const { return Color; }

        CS_COMPONENT_TYPE(EComponentType::SpriteRenderer);
    };

    struct CameraComponent : public IComponent
    {
        SceneCamera Camera;
        bool        Primary = true;
        bool        FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        void Reset() override
        {
            Primary = true;
            FixedAspectRatio = false;
        }

        CS_COMPONENT_TYPE(EComponentType::Camera);
    };

    struct NativeScriptComponent : public IComponent
    {
        Ref<NativeScript> Instance  = nullptr;
        String            ClassName = "";

        NativeScriptComponent()                             = default;
        NativeScriptComponent(const NativeScriptComponent&) = default;

        void Reset() override
        {
            Instance  = nullptr;
            ClassName = "";
        }

        CS_COMPONENT_TYPE(EComponentType::NativeScript);
    };

}
