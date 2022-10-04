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
        CS_DISPATCH_EVENT(WindowResizeEvent, OnWindowResize);
    }

    void OrthographicCameraController::OnMouseScroll(const MouseScrollEvent& e)
    {
        CS_PROFILE_FN();

        mZoomLevel -= e.GetOffset() * 0.25f * mZoomLevel;
        mZoomLevel = std::max(mZoomLevel, 0.25f);
        mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
    }

    void OrthographicCameraController::OnWindowResize(const WindowResizeEvent& e)
    {
        CS_PROFILE_FN();

        mAspectRatio = (float32)e.GetWidth() / (float32)e.GetHeight();
        mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
    }

}