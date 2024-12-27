#pragma once
#include "Base/Types.hpp"
#include "ECS/SceneCamera.hpp"
#include "Event/Events.hpp"
#include "Event/Type/WindowEvents.hpp"
#include "Time/Time.hpp"

#include <glm/glm.hpp>

namespace Cosmic
{

    class EditorCamera : public SceneCamera
    {
    public:
        EditorCamera();

    public:
        void OnUpdate(Dt dt);
        void OnEvent(const IEvent& e);
        void OnResized(uint32 width, uint32 height);

    public:
        void Enable()  { mEnabled = true;  }
        void Disable() { mEnabled = false; }

    public:
        bool IsEnabled()                const { return mEnabled; }
        const glm::mat4& GetTransform() const { return mView;    }

    private:
        void MovementOrthographic(Dt dt);
        void MovementPerspective(Dt dt);

        void RecalculateViewOrthographic();
        void RecalculateViewPerspective();

    private:
        bool OnMouseScrolled(const MouseScrollEvent& e);
        bool OnMouseMoved(const MouseMoveEvent& e);
        bool OnKeyPressed(const KeyPressEvent& e);

    private:
        bool mEnabled = true;

        float32 mMovementSpeed = 10.0f;

        glm::vec3 mPosition = glm::vec3(0.0f);
        glm::mat4 mView;

        uint32 mWidth, mHeight;
    };
    

}
