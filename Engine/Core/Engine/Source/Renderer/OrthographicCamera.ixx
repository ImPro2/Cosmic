module;
#include "cspch.hpp"
#include <glm/glm.hpp>
export module Cosmic.Renderer.OrthographicCamera;

import Cosmic.Base;

namespace Cosmic
{

    export class OrthographicCamera
    {
    public:
        OrthographicCamera(float32 left = 0.0f, float32 right = 0.0f, float32 bottom = 0.0f, float32 top = 0.0f);

    public:
        void SetProjection(float32 left, float32 right, float32 bottom, float32 top);
        void SetPosition(const glm::vec3& pos) { mPosition = pos; RecalculateViewProjMat(); }
        void SetRotation(float32 rot)          { mRotation = rot; RecalculateViewProjMat(); }

    public:
        const glm::vec3& GetPosition()    const { return mPosition;    }
        float32          GetRotation()    const { return mRotation;    }

        const glm::mat4& GetProjMat()     const { return mProjMat;     }
        const glm::mat4& GetViewMat()     const { return mViewMat;     }
        const glm::mat4& GetViewProjMat() const { return mViewProjMat; }

    private:
        void RecalculateViewProjMat();

    private:
        glm::vec3 mPosition    = glm::vec3(0.0f);
        float32   mRotation    = 0.0f;
        glm::mat4 mViewProjMat = glm::mat4(1.0f);
        glm::mat4 mProjMat     = glm::mat4(1.0f);
        glm::mat4 mViewMat     = glm::mat4(1.0f);
    };

}