#include "cspch.hpp"
#include "EditorCamera.hpp"
#include "App/Input.hpp"
#include "App/KeyAndMouseCodes.hpp"
#include "Time/Time.hpp"

#include <cwchar>
#include <glm/gtc/matrix_transform.hpp>

namespace Cosmic
{

    namespace Utils
    {

        static float2 ScreenCoordsToNDC(const float2& screenCoords, uint32 width, uint32 height)
        {
            return {
                (2.0f * screenCoords.x) / width,
                (2.0f * screenCoords.y) / height
            };
        }

    }

    EditorCamera::EditorCamera()
        : SceneCamera()
    {
        RecalculateViewOrthographic();
    }

    void EditorCamera::OnUpdate()
    {
        switch (mProjectionType)
        {
            case EProjectionType::Orthographic: MovementOrthographic();
            case EProjectionType::Perspective:  MovementPerspective();
        }
    }

    void EditorCamera::OnEvent(const IEvent& e)
    {
        EventDispatcher dispatcher(e);
        CS_DISPATCH_EVENT(MouseScrollEvent, OnMouseScrolled);
        CS_DISPATCH_EVENT(MouseMoveEvent, OnMouseMoved);
        CS_DISPATCH_EVENT(KeyPressEvent, OnKeyPressed);
    }

    void EditorCamera::OnResized(uint32 width, uint32 height)
    {
        mWidth = width;
        mHeight = height;

        SetViewportSize(width, height);
    }

    void EditorCamera::MovementOrthographic()
    {
        glm::vec2 movementDir = {
            (float32)Input::IsKeyPressed(EKeyCode::A) - (float32)Input::IsKeyPressed(EKeyCode::D),
            (float32)Input::IsKeyPressed(EKeyCode::S) - (float32)Input::IsKeyPressed(EKeyCode::W)
        };
        
        if (movementDir.x != 0 || movementDir.y != 0)
        {
            mPosition += glm::vec3(mMovementSpeed * glm::normalize(movementDir) * (float32)Time::GetDeltaTime(), 0.0f);
            RecalculateViewOrthographic();
        }
    }

    void EditorCamera::MovementPerspective()
    {

    }

    void EditorCamera::RecalculateViewOrthographic()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), mPosition);
        mView = glm::inverse(transform);
    }

    void EditorCamera::RecalculateViewPerspective()
    {

    }
    
    bool EditorCamera::OnMouseScrolled(const MouseScrollEvent& e)
    {
        mOrthographicSize -= e.GetOffset() * 0.25f * mOrthographicSize;
        mOrthographicSize = std::max(mOrthographicSize, 0.25f);
        RecalculateProjection();

        return false;
    }

    bool EditorCamera::OnMouseMoved(const MouseMoveEvent& e)
    {
        static float2 prevMousePos = e.GetPosition();
        float2 currMousePos        = e.GetPosition();
        float2 mouseDelta          = { currMousePos.x - prevMousePos.x, currMousePos.y - prevMousePos.y };
        prevMousePos               = currMousePos;

        if (Input::IsMouseButtonPressed(EMouseCode::ButtonRight))
        {
            float2 mouseDeltaNDC = Utils::ScreenCoordsToNDC(mouseDelta, mWidth, mHeight);
            glm::vec4 worldSpaceDelta = mProjectionInverse * glm::vec4(mouseDeltaNDC.x, mouseDeltaNDC.y, 0.0f, 1.0f);

            mPosition += glm::vec3(worldSpaceDelta.x, -worldSpaceDelta.y, 0.0f);
            RecalculateViewOrthographic();
        }

        return false;
    }

    bool EditorCamera::OnKeyPressed(const KeyPressEvent& e)
    {
        return false;
    }

}

