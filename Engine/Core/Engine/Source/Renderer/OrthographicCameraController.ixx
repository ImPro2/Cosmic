module;
#include "cspch.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
export module Cosmic.Renderer.OrthographicCameraController;

import Cosmic.Renderer.OrthographicCamera;
import Cosmic.Base;
import Cosmic.App.Events;
import Cosmic.App.WindowEvents;
import Cosmic.Time;
import Cosmic.App.Input;
import Cosmic.Time.DeltaTime;
import Cosmic.App.Log;

namespace Cosmic
{

    export class OrthographicCameraController
    {
    public:
        OrthographicCameraController() = default;
        OrthographicCameraController(float32 aspectRatio, bool rotation = false);

    public:
        void SetAspectRatio(float32 aspectRatio);
        void SetRotation(bool rotate) { mRotation = rotate; }

    public:
        void OnUpdate();
        void OnEvent(const Event& e);

    public:
        void SetZoomLevel(float32 level) { mZoomLevel = level; }

    public:
        OrthographicCamera& GetCamera() { return mCamera; }
        const OrthographicCamera& GetCamera() const { return mCamera; }
        float32 GetZoomLevel() const { return mZoomLevel; }

    private:
        void OnMouseScroll(const MouseScrollEvent& e);
        void OnWindowResize(const WindowResizeEvent& e);

    private:
        float32 mAspectRatio;
        float32 mZoomLevel = 1.0f;
        OrthographicCamera mCamera;

        bool mRotation = false;

        glm::vec3 mCameraPosition = glm::vec3(0.0f);
        float32 mCameraRotation = 0.0f;
        float32 mCameraTranslationAcceleration = 1.0f;
        float32 mCameraRotationAcceleration = 100.0f;
    };

}