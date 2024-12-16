#include "Texture.hpp"
#include "cspch.hpp"
#include "Renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

CS_MODULE_LOG_INFO(Cosmic, Renderer.Renderer2D);

namespace Cosmic
{

    PersistentRef<Renderer2D> Renderer2D::Init()
    {
        CS_PROFILE_FN();

        sInstance = CreatePersistentRef<Renderer2D>();

        sInstance->mData.QuadVertexBuffer = CreateVertexBuffer(nullptr, sInstance->mData.MaxVertices * sizeof(QuadVertex), EBufferUsage::Dynamic);
        sInstance->mData.QuadVertexBuffer->SetLayout({
            VertexBufferElement(EShaderDataType::Float4),
            VertexBufferElement(EShaderDataType::Float4),
            VertexBufferElement(EShaderDataType::Float2),
            VertexBufferElement(EShaderDataType::Float),
            VertexBufferElement(EShaderDataType::Float),
            VertexBufferElement(EShaderDataType::Int)
        });

        sInstance->mData.QuadVertexBuffer->Bind();

        sInstance->mData.QuadVertexBufferBasePtr = new QuadVertex[sInstance->mData.MaxVertices];

        uint32* quadIndices = new uint32[sInstance->mData.MaxIndices];

        uint32 offset = 0;
        for (int32 i = 0; i < sInstance->mData.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        sInstance->mData.QuadIndexBuffer = CreateIndexBuffer(quadIndices, sInstance->mData.MaxIndices);
        sInstance->mData.QuadIndexBuffer->Bind();
        delete[] quadIndices;

        uint32 whiteTextureData = 0xffffffff;
        Texture2DInfo whiteTextureInfo = Texture2DInfo(1, 1, ETextureWrapMode::Repeat);

        sInstance->mData.WhiteTexture = CreateTexture2D(whiteTextureInfo);
        sInstance->mData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32));

        sInstance->mData.Standard2DShader = CreateShader("Engine/Core/Engine/Assets/Shaders/Standard2D.glsl");
        sInstance->mData.Standard2DShader->Bind();

        for (int32 i = 0; i < sInstance->mData.MaxTextureSlots; i++)
            sInstance->mData.Standard2DShader->SetInt(std::format("uTextures[{}]", i), i);

        sInstance->mData.TextureSlots[0] = sInstance->mData.WhiteTexture;

        sInstance->mData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        sInstance->mData.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        sInstance->mData.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        sInstance->mData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        return sInstance;
    }

    void Renderer2D::Shutdown()
    {
        CS_PROFILE_FN();

        sInstance.Release();
    }

    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        CS_PROFILE_FN();

        glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

        sInstance->mData.Standard2DShader->Bind();
        sInstance->mData.Standard2DShader->SetMat4("uViewProjection", viewProj);

        StartBatch();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        CS_PROFILE_FN();

        sInstance->mData.Standard2DShader->Bind();
        sInstance->mData.Standard2DShader->SetMat4("uViewProjection", camera.GetViewProjMat());

        StartBatch();
    }

    void Renderer2D::EndScene()
    {
        CS_PROFILE_FN();

        Flush();
    }

    void Renderer2D::StartBatch()
    {
        CS_PROFILE_FN();

        sInstance->mData.QuadIndexCount = 0;
        sInstance->mData.QuadVertexBufferPtr = sInstance->mData.QuadVertexBufferBasePtr;

        sInstance->mData.TextureSlotIndex = 1;
    }

    void Renderer2D::NextBatch()
    {
        CS_PROFILE_FN();

        Flush();
        StartBatch();
    }

    void Renderer2D::Flush()
    {
        CS_PROFILE_FN();

        uint32 dataSize = (uint32)((uint8*)sInstance->mData.QuadVertexBufferPtr - (uint8*)sInstance->mData.QuadVertexBufferBasePtr);
        sInstance->mData.QuadVertexBuffer->SetData(sInstance->mData.QuadVertexBufferBasePtr, dataSize);

        for (int32 i = 0; i < sInstance->mData.TextureSlotIndex; i++)
            sInstance->mData.TextureSlots[i]->Bind(i);

        sInstance->mData.Standard2DShader->Bind();
        RenderCommand::Render(EPrimitiveTopology::TriangleIndexed, sInstance->mData.QuadIndexCount);

        sInstance->mData.Stats.DrawCalls++;
    }

    void Renderer2D::RenderQuad(const glm::vec2& position, float32 rotation, const glm::vec2& scale, float4 color)
    {
        RenderQuad(glm::vec3(position.x, position.y, 0.0f), rotation, scale, color);
    }

    void Renderer2D::RenderQuad(const glm::vec3& position, float32 rotation, const glm::vec2& scale, float4 color)
    {
        CS_PROFILE_FN();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

        RenderQuad(transform, color);
    }

    void Renderer2D::RenderQuad(const glm::vec2& position, float32 rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, float4 color, float32 tilingFactor)
    {
        RenderQuad(glm::vec3(position.x, position.y, 0.0f), rotation, scale, texture, color, tilingFactor);
    }

    void Renderer2D::RenderQuad(const glm::vec3& position, float32 rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, float4 color, float32 tilingFactor)
    {
        CS_PROFILE_FN();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

        RenderQuad(transform, texture, color, tilingFactor);
    }

    void Renderer2D::RenderQuad(const glm::mat4& transform, float4 color, int32 entityID)
    {
        CS_PROFILE_FN();

        if (sInstance->mData.QuadIndexCount >= sInstance->mData.MaxIndices)
            NextBatch();

        const float32 textureIndex = 0.0f;
        const float32 tilingFactor = 1.0f;
        const float2  texCoord[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        for (int32 i = 0; i < 4; i++)
        {
            sInstance->mData.QuadVertexBufferPtr->Position     = transform * sInstance->mData.QuadVertexPositions[i];
            sInstance->mData.QuadVertexBufferPtr->Color        = color;
            sInstance->mData.QuadVertexBufferPtr->TexCoord     = texCoord[i];
            sInstance->mData.QuadVertexBufferPtr->TexIndex     = textureIndex;
            sInstance->mData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            sInstance->mData.QuadVertexBufferPtr->EntityID     = entityID;
            sInstance->mData.QuadVertexBufferPtr++;
        }

        sInstance->mData.QuadIndexCount += 6;
        sInstance->mData.Stats.QuadCount++;
        sInstance->mData.Stats.TotalVertexCount += 4;
        sInstance->mData.Stats.TotalIndexCount  += 6;
    }

    void Renderer2D::RenderQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float4 color, float32 tilingFactor, int32 entityID)
    {
        CS_PROFILE_FN();

        if (sInstance->mData.QuadIndexCount >= sInstance->mData.MaxIndices)
            NextBatch();

        float32 textureIndex = 0.0f;
        const float2  texCoord[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        for (uint32 i = 1; i < sInstance->mData.TextureSlotIndex; i++)
        {
            if (sInstance->mData.TextureSlots[i]->GetRendererID() == texture->GetRendererID())
            {
                textureIndex = (float32)i;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = sInstance->mData.TextureSlotIndex;
            sInstance->mData.TextureSlots[sInstance->mData.TextureSlotIndex] = texture;
            sInstance->mData.TextureSlotIndex++;
        }

        for (int32 i = 0; i < 4; i++)
        {
            sInstance->mData.QuadVertexBufferPtr->Position     = transform * sInstance->mData.QuadVertexPositions[i];
            sInstance->mData.QuadVertexBufferPtr->Color        = color;
            sInstance->mData.QuadVertexBufferPtr->TexCoord     = texCoord[i];
            sInstance->mData.QuadVertexBufferPtr->TexIndex     = textureIndex;
            sInstance->mData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            sInstance->mData.QuadVertexBufferPtr->EntityID     = entityID;
            sInstance->mData.QuadVertexBufferPtr++;
        }

        sInstance->mData.QuadIndexCount += 6;
        sInstance->mData.Stats.QuadCount++;
        sInstance->mData.Stats.TotalVertexCount += 4;
        sInstance->mData.Stats.TotalIndexCount  += 6;
    }

    void Renderer2D::ResetStatistics()
    {
        CS_PROFILE_FN();

        memset(&sInstance->mData.Stats, 0, sizeof(Renderer2DStatistics));
    }

    const Renderer2DStatistics& Renderer2D::GetStatistics()
    {
        CS_PROFILE_FN();

        return sInstance->mData.Stats;
    }

    void Renderer2D::SetMaxQuadCount(uint32 count)
    {
        CS_PROFILE_FN();

        sInstance->mData.MaxQuads = count;
        sInstance->mData.MaxVertices = count * 4;
        sInstance->mData.MaxIndices = count * 6;
    }

}
