module;
#include "cspch.hpp"
#include <glm/glm.hpp>
export module Cosmic.Renderer.Camera;

namespace Cosmic
{

    export class Camera
    {
    public:
        Camera() = default;
        Camera(const glm::mat4& projection)
            : mProjection(projection)
        {
        }

    public:
        const glm::mat4& GetProjection() const { return mProjection; }

    protected:
        glm::mat4 mProjection = glm::mat4(1.0f);
    };

}