#include "cspch.hpp"
#include "EntryPoint/EntryPoint.hpp"
#include "App/Log/Log.hpp"

#include <Windows.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <format>

using namespace Cosmic;

CS_MODULE_LOG_INFO(Sandbox, SandboxApp);

import Cosmic.App;
import Cosmic.Base;
import Cosmic.App.IWindow;
import Cosmic.App.WindowInfo;
import Cosmic.App.Events;
import Cosmic.App.KeyAndMouseCodes;
import Cosmic.App.Module;
import Cosmic.Renderer.Buffer;
import Cosmic.Renderer.Shader;
import Cosmic.Renderer.Texture;
import Cosmic.Renderer.RenderCommand;
import Cosmic.Time;
import Cosmic.App.Input;

namespace Cosmic
{

    class DebugModule : public Module
    {
    public:
        void OnInit() override
        {
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

            mVertexBuffer = CreateVertexBuffer(vertices, sizeof(Vertex) * 4);
            mVertexBuffer->SetLayout({
                VertexBufferElement(EShaderDataType::Float2),
                VertexBufferElement(EShaderDataType::Float2),
            });

            mIndexBuffer = CreateIndexBuffer(indices, 6);

            mTexture = CreateTexture2D("C:/dev/Cosmic/Branding/Logos/Logo.png");
            mTexture->Bind(0);

            mShader = CreateShader("C:/dev/Cosmic/Engine/Core/Engine/Assets/Shaders/Standard2D.glsl");
            mShader->Bind();
        }

        void OnUpdate(Dt dt) override
        {
            // clear screen

            RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });
            RenderCommand::Clear();

            // update

            Input(dt);

            glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), mPosition);
            transform = glm::rotate(transform, glm::radians(mRotation), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 view = glm::translate(glm::mat4(1.0f), mCamPos);
            view = glm::scale(view, glm::vec3(mZoomLevel, mZoomLevel, 1));
            glm::mat4 viewProj = proj * view;

            mShader->SetMat4("uViewProjection", viewProj);
            mShader->SetMat4("uTransform", transform);

            // binding

            mShader->Bind();
            mTexture->Bind(0);
            mVertexBuffer->Bind();
            mIndexBuffer->Bind();

            // rendering

            RenderCommand::Render(EPrimitiveTopology::TriangleIndexed, mIndexBuffer->GetCount());

            // print time stuff

            CS_LOG_INFO("Time Info: CurrentTime: ({}), DeltaTime: ({}), LastFrameTime: ({})", (float32)Time::GetTime(), (float32)dt, (float32)Time::GetLastFrameTime());
        }

        void Input(Dt dt)
        {
            glm::vec2 velocity = {
                (float32)Input::IsKeyPressed(EKeyCode::D) - (float32)Input::IsKeyPressed(EKeyCode::A),
                (float32)Input::IsKeyPressed(EKeyCode::W) - (float32)Input::IsKeyPressed(EKeyCode::S)
            };

            glm::vec2 camVelocity = {
                (float32)Input::IsKeyPressed(EKeyCode::Left) - (float32)Input::IsKeyPressed(EKeyCode::Right),
                (float32)Input::IsKeyPressed(EKeyCode::Down) - (float32)Input::IsKeyPressed(EKeyCode::Up)
            };

            mPosition += 10.0f   * dt * glm::vec3(velocity.x, velocity.y, 0.0f);
            mRotation += 1000.0f * dt * ((float32)Input::IsKeyPressed(EKeyCode::Q) - (float32)Input::IsKeyPressed(EKeyCode::E));
            mCamPos   += 10.0f   * dt * glm::vec3(camVelocity.x, camVelocity.y, 0.0f);

            /*
            if (Input::IsKeyPressed(EKeyCode::W)) mPosition.y += 10.0f * dt;
            if (Input::IsKeyPressed(EKeyCode::A)) mPosition.x -= 10.0f * dt;
            if (Input::IsKeyPressed(EKeyCode::S)) mPosition.y -= 10.0f * dt;
            if (Input::IsKeyPressed(EKeyCode::D)) mPosition.x += 10.0f * dt;

            if (Input::IsKeyPressed(EKeyCode::Q)) mRotation += 1000.0f * dt;
            if (Input::IsKeyPressed(EKeyCode::E)) mRotation -= 1000.0f * dt;

            if (Input::IsKeyPressed(EKeyCode::Left))  mCamPos.x += 10.0f * dt;
            if (Input::IsKeyPressed(EKeyCode::Right)) mCamPos.x -= 10.0f * dt;
            if (Input::IsKeyPressed(EKeyCode::Up))    mCamPos.y -= 10.0f * dt;
            if (Input::IsKeyPressed(EKeyCode::Down))  mCamPos.y += 10.0f * dt;
            */
            /*
            switch (e.GetKeyCode())
            {
                case EKeyCode::W: mPosition.y += 10.0f * dt; break;
                case EKeyCode::A: mPosition.x -= 10.0f * dt; break;
                case EKeyCode::S: mPosition.y -= 10.0f * dt; break;
                case EKeyCode::D: mPosition.x += 10.0f * dt; break;
                case EKeyCode::Q: mRotation += 1000.0f * dt; break;
                case EKeyCode::E: mRotation -= 1000.0f * dt; break;

                case EKeyCode::Left:  mCamPos.x += 10.0f * dt; break;
                case EKeyCode::Right: mCamPos.x -= 10.0f * dt; break;
                case EKeyCode::Up:    mCamPos.y -= 10.0f * dt; break;
                case EKeyCode::Down:  mCamPos.y += 10.0f * dt; break;
            }
            */
        }

        void OnMouseScroll(const MouseScrollEvent& e)
        {
            mZoomLevel += e.GetOffset() * Time::GetDeltaTime() * 10.0f;
        }

        void OnEvent(const Event& e) override
        {
            EventDispatcher dispatcher(e);
            CS_DISPATCH_EVENT(MouseScrollEvent, OnMouseScroll);
        }

    private:
        Ref<VertexBuffer> mVertexBuffer;
        Ref<IndexBuffer>  mIndexBuffer;
        Ref<Texture2D>    mTexture;
        Ref<Shader>       mShader;

        float32           mZoomLevel = 1.0f;
        glm::vec3         mPosition = { 0.0f, 0.0f, 0.0f };
        glm::vec3         mCamPos = { 0.0f, 0.0f, 0.0f };
        float32           mRotation = 0.0f;
    };

    class SandboxApp : public Application
    {
    public:
        SandboxApp()
        {
            Init({});
        }

        void OnInit(const ApplicationInitEvent& e)
        {
            ModuleSystem::Add<DebugModule>();
        }

        void OnEvent(const Event& e) override
        {
            Application::OnEvent(e);

            EventDispatcher dispatcher(e);
            CS_DISPATCH_EVENT(ApplicationInitEvent, OnInit);
        }
    };

    Application* CreateApplication()
    {
        return new SandboxApp();
    }

}