module;
#include "cspch.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
module Cosmic.Renderer.Renderer2D;

CS_MODULE_LOG_INFO(Cosmic, Renderer.Renderer2D);

import Cosmic.Renderer.Buffer;
import Cosmic.Renderer.Shader;
import Cosmic.Renderer.RenderCommand;

namespace Cosmic
{

    struct Renderer2DData
    {
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<IndexBuffer>  QuadIndexBuffer;
        Ref<Shader>       Standard2DShader;
        Ref<Texture2D>    WhiteTexture;
    };

    static Renderer2DData* sData = nullptr;

    void Renderer2D::Init()
    {
        CS_PROFILE_FN();

        sData = new Renderer2DData();

        struct Vertex
        {
            float2 position;
            float2 texCoords;
        };

        Vertex vertices[]
        {
            { { -0.5f, -0.5f }, { 0.0f, 0.0f } },
            { {  0.5f, -0.5f }, { 1.0f, 0.0f } },
            { {  0.5f,  0.5f }, { 1.0f, 1.0f } },
            { { -0.5f,  0.5f }, { 0.0f, 1.0f } },
        };

        uint32 indices[] = { 0, 1, 2,   2, 3, 0 };

        sData->QuadVertexBuffer = CreateVertexBuffer(vertices, sizeof(Vertex) * 4);
        sData->QuadVertexBuffer->SetLayout({
            VertexBufferElement(EShaderDataType::Float2),
            VertexBufferElement(EShaderDataType::Float2),
        });
        sData->QuadVertexBuffer->Bind();

        sData->QuadIndexBuffer = CreateIndexBuffer(indices, 6);
        sData->QuadIndexBuffer->Bind();

        uint32 whiteTextureData = 0xffffffff;

        sData->WhiteTexture = CreateTexture2D(1, 1);
        sData->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32));

        sData->Standard2DShader = CreateShader("C:/dev/Cosmic/Engine/Core/Engine/Assets/Shaders/Standard2D.glsl");
        sData->Standard2DShader->Bind();
        sData->Standard2DShader->SetInt("uTexture", 0);
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
    }

    void Renderer2D::EndScene()
    {
        CS_PROFILE_FN();
    }

    void Renderer2D::RenderQuad(const glm::vec2& position, float32 rotation, const glm::vec2& scale, float4 color)
    {
        RenderQuad(glm::vec3(position.x, position.y, 0.0f), rotation, scale, color);
    }

    void Renderer2D::RenderQuad(const glm::vec3& position, float32 rotation, const glm::vec2& scale, float4 color)
    {
        CS_PROFILE_FN();

        sData->WhiteTexture->Bind();

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));

        sData->Standard2DShader->SetMat4("uTransform", transform);
        sData->Standard2DShader->SetFloat4("uColor", color);

        RenderCommand::Render(EPrimitiveTopology::TriangleIndexed, sData->QuadIndexBuffer->GetCount());
    }

    void Renderer2D::RenderQuad(const glm::vec2& position, float32 rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, float4 color)
    {
        RenderQuad(glm::vec3(position.x, position.y, 0.0f), rotation, scale, texture);
    }

    void Renderer2D::RenderQuad(const glm::vec3& position, float32 rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, float4 color)
    {
        CS_PROFILE_FN();

        texture->Bind();

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));

        sData->Standard2DShader->SetMat4("uTransform", transform);
        sData->Standard2DShader->SetFloat4("uColor", color);

        RenderCommand::Render(EPrimitiveTopology::TriangleIndexed, sData->QuadIndexBuffer->GetCount());
    }

}