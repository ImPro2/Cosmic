#include "cspch.hpp"
#include "EntryPoint/EntryPoint.hpp"
#include "App/Log/Log.hpp"

#include <Windows.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <format>
#include <imgui.h>

using namespace Cosmic;

CS_MODULE_LOG_INFO(Sandbox, SandboxApp);

import Cosmic.App;
import Cosmic.App.Application;
import Cosmic.Gui;
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
import Cosmic.Renderer.OrthographicCamera;
import Cosmic.Renderer.OrthographicCameraController;

namespace Cosmic
{

    class DebugModule : public Module
    {
    public:
        void OnInit() override
        {
            CS_PROFILE_FN();

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

            mCameraController.SetAspectRatio(1280.0f / 720.0f);
            mCameraController.SetRotation(true);
        }

        void OnUpdate(Dt dt) override
        {
            CS_PROFILE_FN();

            // clear screen

            RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });
            RenderCommand::Clear();

            // update

            Input(dt);

            mCameraController.OnUpdate();

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), mPosition);
            transform = glm::rotate(transform, glm::radians(mRotation), glm::vec3(0.0f, 0.0f, 1.0f));

            mShader->SetMat4("uViewProjection", mCameraController.GetCamera().GetViewProjMat());
            mShader->SetMat4("uTransform", transform);

            // binding

            mShader->Bind();
            mTexture->Bind(0);
            mVertexBuffer->Bind();
            mIndexBuffer->Bind();

            // rendering

            RenderCommand::Render(EPrimitiveTopology::TriangleIndexed, mIndexBuffer->GetCount());
        }

        void Input(Dt dt)
        {
            CS_PROFILE_FN();

            glm::vec2 velocity = {
                (float32)Input::IsKeyPressed(EKeyCode::L) - (float32)Input::IsKeyPressed(EKeyCode::J),
                (float32)Input::IsKeyPressed(EKeyCode::I) - (float32)Input::IsKeyPressed(EKeyCode::K)
            };

            mPosition += 10.0f   * dt * glm::vec3(velocity.x, velocity.y, 0.0f);
            mRotation += 1000.0f * dt * ((float32)Input::IsKeyPressed(EKeyCode::U) - (float32)Input::IsKeyPressed(EKeyCode::O));
        }

        void OnWindowResize(const WindowResizeEvent& e)
        {
            float32 width  = (float32)e.GetWidth();
            float32 height = (float32)e.GetHeight();
            float32 aspectRatio = width / height;
            CS_LOG_TRACE("[{}:{}]: {}", width, height, aspectRatio);
        }

        void OnEvent(const Event& e) override
        {
            CS_PROFILE_FN();

            mCameraController.OnEvent(e);

            EventDispatcher dispatcher(e);

            CS_DISPATCH_EVENT(WindowResizeEvent, OnWindowResize);
        }

        void OnImGuiRender() override
        {
            ImGui::ShowDemoWindow();
            ImGui::Begin("SandboxApp Properties");
            {
                ImGui::DragFloat3("Position", glm::value_ptr(mPosition), 0.1f, -10.0f, 10.0f);
                ImGui::DragFloat("Rotation", &mRotation);

                ImGui::Text("Delta Time: %fms", (float32)Time::GetDeltaTime());
                ImGui::Text("FPS: %fms", (float32)Time::GetFPS());
                ImGui::Text("Current Time: %fms", (float32)Time::GetTime());
            }
            ImGui::End();
        }

    private:
        Ref<VertexBuffer> mVertexBuffer;
        Ref<IndexBuffer>  mIndexBuffer;
        Ref<Texture2D>    mTexture;
        Ref<Shader>       mShader;

        glm::vec3         mPosition = { 0.0f, 0.0f, 0.0f };
        float32           mRotation = 0.0f;
        OrthographicCameraController mCameraController;
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