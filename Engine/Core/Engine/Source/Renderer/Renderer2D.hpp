#pragma once
#include <glm/glm.hpp>

#include "Base/Base.hpp"
#include "Renderer/OrthographicCamera.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/Buffer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Shader.hpp"

namespace Cosmic
{

    struct Renderer2DStatistics
    {
        uint32 DrawCalls = 0;
        uint32 QuadCount = 0;

        uint32 TotalVertexCount = 0;
        uint32 TotalIndexCount = 0;
    };

    // High level method of rendering to the screen.
    class Renderer2D : public IRefCounted
    {
    private:
        static PersistentRef<Renderer2D> Init();
        static void                      Shutdown();

    public:
        static void BeginScene(const Camera& camera, const glm::mat4& transform);
        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

    public:
        // TODO: Maybe move transform calculations to the GPU in the vertex shader or maybe compute shader?
        static void RenderQuad(const glm::vec2& position, float32 rotation, const glm::vec2& scale, float4 color);
        static void RenderQuad(const glm::vec3& position, float32 rotation, const glm::vec2& scale, float4 color);
        static void RenderQuad(const glm::vec2& position, float32 rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, float4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, float32 tilingFactor = 1.0f);
        static void RenderQuad(const glm::vec3& position, float32 rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, float4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, float32 tilingFactor = 1.0f);
        static void RenderQuad(const glm::mat4& transform, float4 color, int32 entityID = 0);
        static void RenderQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float4 color, float32 tilingFactor, int32 entityID = 0);

    public:
        static void  ResetStatistics();
        static const Renderer2DStatistics& GetStatistics();

        static void SetMaxQuadCount(uint32 count);

    private:
        static void Flush();
        static void StartBatch();
        static void NextBatch();

    private:
		struct QuadVertex
		{
			glm::vec4 Position;
			float4    Color;
			float2    TexCoord;
			float32   TexIndex;
			float32   TilingFactor;
			int32     EntityID;
		};

		struct Renderer2DData
		{
			uint32 MaxQuads = 10000;
			uint32 MaxVertices = MaxQuads * 4;
			uint32 MaxIndices = MaxQuads * 6;
			static const uint32 MaxTextureSlots = 16; // TODO: RenderCaps

			Ref<VertexBuffer> QuadVertexBuffer;
			Ref<IndexBuffer>  QuadIndexBuffer;
			Ref<Shader>       Standard2DShader;
			Ref<Texture2D>    WhiteTexture;

			uint32 QuadIndexCount = 0;
			QuadVertex* QuadVertexBufferBasePtr = nullptr;
			QuadVertex* QuadVertexBufferPtr = nullptr;

			std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
			uint32 TextureSlotIndex = 1; // 0 is the white texture

			glm::vec4 QuadVertexPositions[4];

			Renderer2DStatistics Stats;
		};

        Renderer2DData mData;

        inline static PersistentRef<Renderer2D> sInstance;

        friend class Application;
    };

}
