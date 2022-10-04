module;
#include "cspch.hpp"
#include <glm/glm.hpp>
export module Cosmic.Renderer.Renderer2D;

import Cosmic.Base;
import Cosmic.Renderer.OrthographicCamera;
import Cosmic.Renderer.Texture;

namespace Cosmic
{

    // High level method of rendering to the screen.
    export class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        // Primitives

        static void RenderQuad(const glm::vec2& position, float32 rotation, const glm::vec2& scale, float4 color);
        static void RenderQuad(const glm::vec3& position, float32 rotation, const glm::vec2& scale, float4 color);
        static void RenderQuad(const glm::vec2& position, float32 rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, float4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
        static void RenderQuad(const glm::vec3& position, float32 rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, float4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
    };

}
