module;
#include "cspch.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
module Cosmic.Renderer.OrthographicCamera;

namespace Cosmic
{

    OrthographicCamera::OrthographicCamera(float32 left, float32 right, float32 bottom, float32 top)
        : mProjMat(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
    {
        CS_PROFILE_FN();

        mViewProjMat = mProjMat * mViewMat;
    }

    void OrthographicCamera::SetProjection(float32 left, float32 right, float32 bottom, float32 top)
    {
        CS_PROFILE_FN();

        mProjMat     = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        mViewProjMat = mProjMat * mViewMat;
    }

    void OrthographicCamera::RecalculateViewProjMat()
    {
        CS_PROFILE_FN();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), mPosition) * glm::rotate(glm::mat4(1.0f), glm::radians(mRotation), glm::vec3(0.0f, 0.0f, 1.0f));
        mViewMat            = glm::inverse(transform);
        mViewProjMat        = mProjMat * mViewMat;
    }

}