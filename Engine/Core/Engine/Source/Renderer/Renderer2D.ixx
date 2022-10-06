module;
#include "cspch.hpp"
#include <glm/glm.hpp>
export module Cosmic.Renderer.Renderer2D;

import Cosmic.Base;
import Cosmic.Renderer.OrthographicCamera;
import Cosmic.Renderer.Texture;

namespace Cosmic
{

    export struct Renderer2DStatistics
    {
        uint32 DrawCalls = 0;
        uint32 QuadCount = 0;

        uint32 TotalVertexCount = 0;
        uint32 TotalIndexCount = 0;
    };

    // High level method of rendering to the screen.
    export class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

    public:
        // TODO: Maybe move transform calculations to the GPU in the vertex shader or maybe compute shader?
        static void RenderQuad(const glm::vec2& position, float32 rotation, const glm::vec2& scale, float4 color);
        static void RenderQuad(const glm::vec3& position, float32 rotation, const glm::vec2& scale, float4 color);
        static void RenderQuad(const glm::vec2& position, float32 rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, float4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, float32 tilingFactor = 1.0f);
        static void RenderQuad(const glm::vec3& position, float32 rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, float4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, float32 tilingFactor = 1.0f);
        static void RenderQuad(const glm::mat4& transform, float4 color);
        static void RenderQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float4 color, float32 tilingFactor);
    public:
        static void  ResetStatistics();
        static const Renderer2DStatistics& GetStatistics();

        static void SetMaxQuadCount(uint32 count);

    private:
        static void Flush();
        static void StartBatch();
        static void NextBatch();
    };

}
