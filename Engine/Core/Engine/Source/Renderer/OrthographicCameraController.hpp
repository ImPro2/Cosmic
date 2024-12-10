#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer/OrthographicCamera.hpp"
#include "Base/Base.hpp"
#include "Event/Events.hpp"
#include "Event/Type/WindowEvents.hpp"
#include "Time/Time.hpp"
#include "App/Input.hpp"
#include "Time/DeltaTime.hpp"
#include "App/Log/Log.hpp"

namespace Cosmic
{

    class OrthographicCameraController
    {
    public:
        OrthographicCameraController() = default;
        OrthographicCameraController(float32 aspectRatio, bool rotation = false);

    public:
        void SetAspectRatio(float32 aspectRatio);
        void SetRotation(bool rotate) { mRotation = rotate; }

    public:
        void OnUpdate();
        void OnEvent(const IEvent& e);
        void OnResize(uint32 width, uint32 height);

    public:
        void SetZoomLevel(float32 level) { mZoomLevel = level; }

    public:
        OrthographicCamera& GetCamera() { return mCamera; }
        const OrthographicCamera& GetCamera() const { return mCamera; }
        float32 GetZoomLevel() const { return mZoomLevel; }

    private:
        bool OnMouseScroll(const MouseScrollEvent& e);
        bool OnMouseMove(const MouseMoveEvent& e);

    private:
        float32 mAspectRatio;
        float32 mZoomLevel = 1.0f;
        OrthographicCamera mCamera;

        bool mRotation = false;

        glm::vec3 mCameraPosition = glm::vec3(0.0f);
        float32 mCameraRotation = 0.0f;
        float32 mCameraTranslationAcceleration = 2.0f;
        float32 mCameraRotationAcceleration = 100.0f;

        float32 mWidth, mHeight;
    };

}