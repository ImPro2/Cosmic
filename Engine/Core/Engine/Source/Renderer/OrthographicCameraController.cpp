module;
#include "cspch.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
module Cosmic.Renderer.OrthographicCameraController;
CS_MODULE_LOG_INFO(Cosmic, Renderer.OrthographicCameraController);

namespace Cosmic
{

    OrthographicCameraController::OrthographicCameraController(float32 aspectRatio, bool rotation)
        : mAspectRatio(aspectRatio),
          mCamera(-mAspectRatio * mZoomLevel, mAspectRatio* mZoomLevel, -mZoomLevel, mZoomLevel),
          mRotation(rotation)
    {
        CS_PROFILE_FN();
    }

    void OrthographicCameraController::SetAspectRatio(float32 aspectRatio)
    {
        mAspectRatio = aspectRatio;
        mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
    }

    void OrthographicCameraController::OnUpdate()
    {
        CS_PROFILE_FN();

        Dt dt = Time::GetDeltaTime();

        glm::vec2 velocity = {
            (float32)Input::IsKeyPressed(EKeyCode::D) - (float32)Input::IsKeyPressed(EKeyCode::A),
            (float32)Input::IsKeyPressed(EKeyCode::W) - (float32)Input::IsKeyPressed(EKeyCode::S)
        };

        glm::normalize(velocity);

        mCameraTranslationAcceleration = Input::IsKeyPressed(EKeyCode::LeftShift) ? 2.0f : 1.0f;

        mCameraPosition += mZoomLevel * mCameraTranslationAcceleration * dt * glm::vec3(velocity.x, velocity.y, 0.0f);

        if (mRotation)
        {
            mCameraRotation += mCameraRotationAcceleration * dt * ((float32)Input::IsKeyPressed(EKeyCode::Q) - (float32)Input::IsKeyPressed(EKeyCode::E));
            mCamera.SetRotation(mCameraRotation);
        }

        mCamera.SetPosition(mCameraPosition);
    }

    void OrthographicCameraController::OnEvent(const Event& e)
    {
        CS_PROFILE_FN();

        EventDispatcher dispatcher(e);

        CS_DISPATCH_EVENT(MouseScrollEvent, OnMouseScroll);
        CS_DISPATCH_EVENT(MouseMoveEvent, OnMouseMove);
    }

    void OrthographicCameraController::OnResize(uint32 width, uint32 height)
    {
        CS_PROFILE_FN();

        mWidth = width;
        mHeight = height;
        mAspectRatio = (float32)width / (float32)height;
        mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
    }

    bool OrthographicCameraController::OnMouseScroll(const MouseScrollEvent& e)
    {
        CS_PROFILE_FN();

        mZoomLevel -= e.GetOffset() * 0.25f * mZoomLevel;
        mZoomLevel = std::max(mZoomLevel, 0.25f);
        mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);

        return false;
    }

    bool OrthographicCameraController::OnMouseMove(const MouseMoveEvent& e)
    {
        static float2 previousMousePosition = {
            (e.GetPosition().x / mWidth)  * mAspectRatio * mZoomLevel,
            (e.GetPosition().y / mHeight) * -mZoomLevel
        };

        float2 currentMousePosition = {
            (e.GetPosition().x / mWidth)  * mAspectRatio * mZoomLevel,
            (e.GetPosition().y / mHeight) * -mZoomLevel
        };

        glm::vec3 changeInMousePosition = { previousMousePosition.x - currentMousePosition.x, previousMousePosition.y - currentMousePosition.y, 0.0f };

        if (Input::IsMouseButtonPressed(EMouseCode::ButtonRight))
        {
            mCameraPosition += (float32)Time::GetDeltaTime() * 400.0f * changeInMousePosition;
            mCamera.SetPosition(mCameraPosition);
        }

        previousMousePosition = currentMousePosition;

        return false;
    }

}