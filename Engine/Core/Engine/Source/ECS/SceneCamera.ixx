module;
#include "cspch.hpp"
#include <glm/glm.hpp>
export module Cosmic.ECS.SceneCamera;

import Cosmic.Renderer.Camera;
import Cosmic.Base;

namespace Cosmic
{

    export enum class EProjectionType
    {
        Perspective  = 0,
        Orthographic = 1
    };

    export class SceneCamera : public Camera
    {
    public:
        SceneCamera();

    public:
        void SetPerspective(float32 fov, float32 nearClip, float32 farClip);
        void SetOrthographic(float32 size, float32 nearClip, float32 farClip);
        void SetOrthographicAndPerspective(EProjectionType projType, float32 orthoSize, float32 orthoNear, float32 orthoFar, float32 persFOV, float32 persNear, float32 persFar);
        void SetViewportSize(uint32 width, uint32 height);
        void SetAspectRatio(float32 aspectRatio);

        void SetPerspectiveVerticalFOV(float32 fov)  { mPerspectiveFOV  = fov;   RecalculateProjection(); }
        void SetPerspectiveNearClip(float32 near)    { mPerspectiveNear = near;  RecalculateProjection(); }
        void SetPerspectiveFarClip(float32 far)      { mPerspectiveFar  = far;   RecalculateProjection(); }

        void SetOrthographicSize(float32 size)       { mOrthographicSize = size; RecalculateProjection(); }
        void SetOrthographicNearClip(float32 near)   { mOrthographicNear = near; RecalculateProjection(); }
        void SetOrthographicFarClip(float32 far)     { mOrthographicFar  = far;  RecalculateProjection(); }

        void SetProjectionType(EProjectionType type) { mProjectionType = type;                            }

    public:
        float32 GetPerspectiveVerticalFOV() const { return mPerspectiveFOV;   }
        float32 GetPerspectiveNearClip()    const { return mPerspectiveNear;  }
        float32 GetPerspectiveFarClip()     const { return mPerspectiveFar;   }

        float32 GetOrthographicSize()       const { return mOrthographicSize; }
        float32 GetOrthographicNearClip()   const { return mOrthographicNear; }
        float32 GetOrthographicFarClip()    const { return mOrthographicFar;  }

        float32 GetAspectRatio()            const { return mAspectRatio;      }
        EProjectionType GetProjectionType() const { return mProjectionType;   }

    private:
        void RecalculateProjection();

    private:
        EProjectionType mProjectionType = EProjectionType::Orthographic;

        float32 mOrthographicSize =  10.0f;
        float32 mOrthographicNear = -1.0f;
        float32 mOrthographicFar  =  1.0f;

        float32 mPerspectiveFOV  = glm::radians(45.0f);
        float32 mPerspectiveNear = 0.01f;
        float32 mPerspectiveFar  = 1000.0f;

        float32 mAspectRatio = 1.6f;
    };

}