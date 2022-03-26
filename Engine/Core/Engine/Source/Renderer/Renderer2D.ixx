module;
#include "cspch.hpp"
#include <glm/glm.hpp>
export module Cosmic.Renderer.Renderer2D;

import Cosmic.Base;

namespace Cosmic
{

    // High level method of rendering to the screen.
    export class Renderer2D
    {
    public:
        void Init();
        void Shutdown();

        void Begin();
        void End();

        void RenderQuad(const glm::vec2& position, const glm::vec2& size, float4 color);
        void RenderQuad(const glm::vec3& position, const glm::vec2& size, float4 color);

        void RenderRotatedQuad(const glm::vec2& position, const glm::vec2& size, float4 color, float32 rotation);
        void RenderRotatedQuad(const glm::vec3& position, const glm::vec2& size, float4 color, float32 rotation);
    };

}