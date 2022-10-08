module;
#include "cspch.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
module Cosmic.ECS.SceneCamera;

namespace Cosmic
{

    SceneCamera::SceneCamera()
    {
        RecalculateProjection();
    }

    void SceneCamera::SetPerspective(float32 fov, float32 nearClip, float32 farClip)
    {
        mProjectionType = EProjectionType::Perspective;

        mPerspectiveFOV  = fov;
        mPerspectiveNear = nearClip;
        mPerspectiveFar  = farClip;

        RecalculateProjection();
    }

    void SceneCamera::SetOrthographic(float32 size, float32 nearClip, float32 farClip)
    {
        mProjectionType = EProjectionType::Orthographic;

        mOrthographicSize = size;
        mOrthographicNear = nearClip;
        mOrthographicFar  = farClip;

        RecalculateProjection();
    }

    void SceneCamera::SetViewportSize(uint32 width, uint32 height)
    {
        mAspectRatio = (float32)width / (float32)height;

        RecalculateProjection();
    }

    void SceneCamera::RecalculateProjection()
    {
        if (mProjectionType == EProjectionType::Perspective)
        {
            mProjection = glm::perspective(mPerspectiveFOV, mAspectRatio, mPerspectiveNear, mPerspectiveFar);
        }
        else if (mProjectionType == EProjectionType::Orthographic)
        {
            float32 orthoLeft   = -mOrthographicSize * mAspectRatio * 0.5f;
            float32 orthoRight  =  mOrthographicSize * mAspectRatio * 0.5f;
            float32 orthoBottom = -mOrthographicSize * 0.5f;
            float32 orthoTop    =  mOrthographicSize * 0.5f;

            mProjection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, mOrthographicNear, mOrthographicFar);
        }

    }


}