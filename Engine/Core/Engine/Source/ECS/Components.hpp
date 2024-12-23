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

#define CS_COMPONENT_TYPE(type)                                      \
	EComponentType        GetType() const override { return type; }  \
	static EComponentType GetStaticType()          { return type; }


namespace Cosmic
{

    enum class EComponentType
    {
        EntityMetadata, Transform, SpriteRenderer, Camera, NativeScript
    };

    struct IComponent
    {
        virtual void           Reset()         = 0;
        virtual EComponentType GetType() const = 0;
    };

    struct EntityMetadataComponent : public IComponent
    {
        int32  ID        = -1;
        String Tag       = "";
        bool   IsVisible = true;

        size_t ChildrenCount = 0;
        Entity FirstChild, Next, Prev, Parent;
    
        EntityMetadataComponent() = default;
        EntityMetadataComponent(const EntityMetadataComponent&) = default;

        void Reset() override
        {
            ID        = -1;
            Tag       = "";
            IsVisible = true;

            ChildrenCount = 0;

            FirstChild = Entity();
            Next       = Entity();
            Prev       = Entity();
            Parent     = Entity();
        }

        CS_COMPONENT_TYPE(EComponentType::EntityMetadata);
    };

    class Scene;

    struct TransformComponent : public IComponent
    {
        bool IsRelative = true;

        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation    = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale       = { 1.0f, 1.0f, 1.0f };

        const glm::vec3& GetAbsoluteTranslation() const { return mAbsoluteTranslation; }
        const glm::vec3& GetAbsoluteRotation()    const { return mAbsoluteRotation;    }
        const glm::vec3& GetAbsoluteScale()       const { return mAbsoluteScale;       }

    public:
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

        glm::mat4 GetAbsoluteTransform() const
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(mAbsoluteRotation));

            return glm::translate(glm::mat4(1.0f), mAbsoluteTranslation) * rotation * glm::scale(glm::mat4(1.0f), mAbsoluteScale);
        }

        void Reset() override
        {
            IsRelative = true;

            Translation = { 0.0f, 0.0f, 0.0f };
            Rotation    = { 0.0f, 0.0f, 0.0f };
            Scale       = { 1.0f, 1.0f, 1.0f };

            mAbsoluteTranslation = { 0.0f, 0.0f, 0.0f };
            mAbsoluteRotation    = { 0.0f, 0.0f, 0.0f };
            mAbsoluteScale       = { 1.0f, 1.0f, 1.0f };
        }

        CS_COMPONENT_TYPE(EComponentType::Transform);

    private:
        glm::vec3 mAbsoluteTranslation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 mAbsoluteRotation    = { 0.0f, 0.0f, 0.0f };
        glm::vec3 mAbsoluteScale       = { 1.0f, 1.0f, 1.0f };

        friend class Scene;
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
        WeakRef<NativeScript> Instance   = nullptr;
        String                ClassName  = "";
        bool                  ShouldLoad = false;

        NativeScriptComponent()                             = default;
        NativeScriptComponent(const NativeScriptComponent&) = default;

        void Reset() override
        {
            Instance.Reset();
            ClassName  = "";
            ShouldLoad = false;
        }

        CS_COMPONENT_TYPE(EComponentType::NativeScript);
    };

}
