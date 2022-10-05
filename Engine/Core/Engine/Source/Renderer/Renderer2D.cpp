module;
#include "cspch.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
module Cosmic.Renderer.Renderer2D;

CS_MODULE_LOG_INFO(Cosmic, Renderer.Renderer2D);

import Cosmic.Renderer.Buffer;
import Cosmic.Renderer.Shader;
import Cosmic.Renderer.RenderCommand;

namespace Cosmic
{

    struct QuadVertex
    {
        float3  Position;
        float4  Color;
        float2  TexCoord;
        float32 TexIndex;
        float32 TilingFactor;
    };

    struct Renderer2DData
    {
        const uint32 MaxQuads = 10000;
        const uint32 MaxVertices = MaxQuads * 4;
        const uint32 MaxIndices = MaxQuads * 6;
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
    };

    static Renderer2DData* sData = nullptr;

    void Renderer2D::Init()
    {
        CS_PROFILE_FN();

        sData = new Renderer2DData();

        sData->QuadVertexBuffer = CreateVertexBuffer(nullptr, sData->MaxVertices * sizeof(QuadVertex), EBufferUsage::Dynamic);
        sData->QuadVertexBuffer->SetLayout({
            VertexBufferElement(EShaderDataType::Float3),
            VertexBufferElement(EShaderDataType::Float4),
            VertexBufferElement(EShaderDataType::Float2),
            VertexBufferElement(EShaderDataType::Float),
            VertexBufferElement(EShaderDataType::Float)
        });
        sData->QuadVertexBuffer->Bind();

        sData->QuadVertexBufferBasePtr = new QuadVertex[sData->MaxVertices];

        uint32* quadIndices = new uint32[sData->MaxIndices];

        uint32 offset = 0;
        for (int32 i = 0; i < sData->MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        sData->QuadIndexBuffer = CreateIndexBuffer(quadIndices, sData->MaxIndices);
        sData->QuadIndexBuffer->Bind();
        delete[] quadIndices;

        uint32 whiteTextureData = 0xffffffff;

        sData->WhiteTexture = CreateTexture2D(1, 1);
        sData->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32));

        sData->Standard2DShader = CreateShader("C:/dev/Cosmic/Engine/Core/Engine/Assets/Shaders/Standard2D.glsl");
        sData->Standard2DShader->Bind();

        for (int32 i = 0; i < sData->MaxTextureSlots; i++)
            sData->Standard2DShader->SetInt(std::format("uTextures[{}]", i), i);

        sData->TextureSlots[0] = sData->WhiteTexture;
    }

    void Renderer2D::Shutdown()
    {
        CS_PROFILE_FN();

        delete sData;
        sData = nullptr;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        CS_PROFILE_FN();

        sData->Standard2DShader->Bind();
        sData->Standard2DShader->SetMat4("uViewProjection", camera.GetViewProjMat());

        sData->QuadIndexCount = 0;
        sData->QuadVertexBufferPtr = sData->QuadVertexBufferBasePtr;

        sData->TextureSlotIndex = 1;
    }

    void Renderer2D::EndScene()
    {
        CS_PROFILE_FN();

        uint32 dataSize = (uint8*)sData->QuadVertexBufferPtr - (uint8*)sData->QuadVertexBufferBasePtr;
        sData->QuadVertexBuffer->SetData(sData->QuadVertexBufferBasePtr, dataSize);

        Flush();
    }

    void Renderer2D::Flush()
    {
        CS_PROFILE_FN();

        for (int32 i = 0; i < sData->TextureSlotIndex; i++)
            sData->TextureSlots[i]->Bind(i);

        RenderCommand::Render(EPrimitiveTopology::TriangleIndexed, sData->QuadIndexCount);
    }

    void Renderer2D::RenderQuad(const glm::vec2& position, float32 rotation, const glm::vec2& scale, float4 color)
    {
        RenderQuad(glm::vec3(position.x, position.y, 0.0f), rotation, scale, color);
    }

    void Renderer2D::RenderQuad(const glm::vec3& position, float32 rotation, const glm::vec2& scale, float4 color)
    {
        CS_PROFILE_FN();

        // bottom left
        sData->QuadVertexBufferPtr->Position = { position.x - 0.5f * scale.x, position.y - 0.5f * scale.y, position.z };
        sData->QuadVertexBufferPtr->Color    = color;
        sData->QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        sData->QuadVertexBufferPtr->TexIndex = 0.0f; // slot 0 is the white texture

        sData->QuadVertexBufferPtr++;

        // bottom right
        sData->QuadVertexBufferPtr->Position = { position.x + 0.5f * scale.x, position.y - 0.5f * scale.y, position.z };
        sData->QuadVertexBufferPtr->Color = color;
        sData->QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        sData->QuadVertexBufferPtr->TexIndex = 0.0f;
        sData->QuadVertexBufferPtr++;

        // top right
        sData->QuadVertexBufferPtr->Position = { position.x + 0.5f * scale.x, position.y + 0.5f * scale.y, position.z };
        sData->QuadVertexBufferPtr->Color = color;
        sData->QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        sData->QuadVertexBufferPtr->TexIndex = 0.0f;
        sData->QuadVertexBufferPtr++;

        // top left
        sData->QuadVertexBufferPtr->Position = { position.x - 0.5f * scale.x, position.y + 0.5f * scale.y, position.z };
        sData->QuadVertexBufferPtr->Color = color;
        sData->QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        sData->QuadVertexBufferPtr->TexIndex = 0.0f;
        sData->QuadVertexBufferPtr++;

        sData->QuadIndexCount += 6;

#if 0
        sData->WhiteTexture->Bind();

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));

        sData->Standard2DShader->SetMat4("uTransform", transform);
        sData->Standard2DShader->SetFloat4("uColor", color);

        RenderCommand::Render(EPrimitiveTopology::TriangleIndexed, sData->QuadIndexBuffer->GetCount());
#endif
    }

    void Renderer2D::RenderQuad(const glm::vec2& position, float32 rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, float4 color, float32 tilingFactor)
    {
        RenderQuad(glm::vec3(position.x, position.y, 0.0f), rotation, scale, texture, color, tilingFactor);
    }

    void Renderer2D::RenderQuad(const glm::vec3& position, float32 rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, float4 color, float32 tilingFactor)
    {
        CS_PROFILE_FN();

        float32 textureIndex = 0.0f;

        for (uint32 i = 1; i < sData->TextureSlotIndex; i++)
        {
            if (sData->TextureSlots[i]->GetRendererID() == texture->GetRendererID())
            {
                textureIndex = (float32)i;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = sData->TextureSlotIndex;
            sData->TextureSlots[sData->TextureSlotIndex] = texture;
            sData->TextureSlotIndex++;
        }

        // bottom left
        sData->QuadVertexBufferPtr->Position = { position.x - 0.5f * scale.x, position.y - 0.5f * scale.y, position.z };
        sData->QuadVertexBufferPtr->Color = color;
        sData->QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        sData->QuadVertexBufferPtr->TexIndex = textureIndex;
        sData->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        sData->QuadVertexBufferPtr++;

        // bottom right
        sData->QuadVertexBufferPtr->Position = { position.x + 0.5f * scale.x, position.y - 0.5f * scale.y, position.z };
        sData->QuadVertexBufferPtr->Color = color;
        sData->QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        sData->QuadVertexBufferPtr->TexIndex = textureIndex;
        sData->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        sData->QuadVertexBufferPtr++;

        // top right
        sData->QuadVertexBufferPtr->Position = { position.x + 0.5f * scale.x, position.y + 0.5f * scale.y, position.z };
        sData->QuadVertexBufferPtr->Color = color;
        sData->QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        sData->QuadVertexBufferPtr->TexIndex = textureIndex;
        sData->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        sData->QuadVertexBufferPtr++;

        // top left
        sData->QuadVertexBufferPtr->Position = { position.x - 0.5f * scale.x, position.y + 0.5f * scale.y, position.z };
        sData->QuadVertexBufferPtr->Color = color;
        sData->QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        sData->QuadVertexBufferPtr->TexIndex = textureIndex;
        sData->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        sData->QuadVertexBufferPtr++;

        sData->QuadIndexCount += 6;

#if 0
        texture->Bind();

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));

        sData->Standard2DShader->SetMat4("uTransform", transform);
        sData->Standard2DShader->SetFloat4("uColor", color);

        RenderCommand::Render(EPrimitiveTopology::TriangleIndexed, sData->QuadIndexBuffer->GetCount());
#endif
    }

}