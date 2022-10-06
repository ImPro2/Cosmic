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
import Cosmic.Time.DeltaTime;
import Cosmic.Renderer.Renderer2D;
import Cosmic.Renderer.Framebuffer;

namespace Cosmic
{

    class DebugModule : public Module
    {
    public:
        void OnInit() override
        {
            CS_PROFILE_FN();

            mCameraController.SetAspectRatio(1280.0f / 720.0f);
            mCameraController.SetRotation(true);

            mCosmicLogoTexture = CreateTexture2D("C:/dev/Cosmic/Branding/Logos/Logo.png");

            FramebufferInfo fbInfo = {};
            fbInfo.Width = 1280.0f;
            fbInfo.Height = 720.0f;
            fbInfo.SwapChainTarget = false;
            mFramebuffer = CreateFramebuffer(fbInfo);
        }

        void OnUpdate(Dt dt) override
        {
            CS_PROFILE_FN();

            // Update

            mCameraController.OnUpdate();
            Input(dt);
            Renderer2D::ResetStatistics();

            // Render

            mFramebuffer->Bind();
            RenderCommand::SetClearColor(mClearColor);
            RenderCommand::Clear();

            Renderer2D::BeginScene(mCameraController.GetCamera());

            //Renderer2D::RenderQuad(mPosition, mRotation, mScale, mColor);
            for (int32 y = 0; y < mVerticalQuadCount; y++)
            {
                for (int32 x = 0; x < mHorizontalQuadCount; x++)
                {
                    Renderer2D::RenderQuad(mPosition + mQuadSpacing * glm::vec3(x, y, 0.0f), mRotation, mScale, mCosmicLogoTexture, mColor);
                }
            }

            Renderer2D::EndScene();

            mFramebuffer->Unbind();
        }

        void Input(Dt dt)
        {
            CS_PROFILE_FN();

            glm::vec2 velocity = {
                (float32)Input::IsKeyPressed(EKeyCode::L) - (float32)Input::IsKeyPressed(EKeyCode::J),
                (float32)Input::IsKeyPressed(EKeyCode::I) - (float32)Input::IsKeyPressed(EKeyCode::K)
            };

            mPosition += mPositionAcceleration * dt * glm::vec3(velocity.x, velocity.y, 0.0f);
            mRotation += mRotationAcceleration * dt * ((float32)Input::IsKeyPressed(EKeyCode::U) - (float32)Input::IsKeyPressed(EKeyCode::O));
        }

        void OnEvent(const Event& e) override
        {
            CS_PROFILE_FN();

            mCameraController.OnEvent(e);
        }

        void OnImGuiRender() override
        {
            CS_PROFILE_FN();

            ImGui::ShowDemoWindow();
            ImGui::Begin("SandboxApp Properties");
            {
                ImGui::DragFloat2("Quad Position", glm::value_ptr(mPosition), 0.1f, -10.0f, 10.0f);
                ImGui::DragFloat("Quad Rotation", &mRotation, 0.1f);
                ImGui::DragFloat2("Quad Scale", glm::value_ptr(mScale), 0.1f, -10.0f, 10.0f);
                ImGui::DragFloat("Quad Position Acceleration", &mPositionAcceleration, 0.1f, -100.0f, 100.0f);
                ImGui::DragFloat("Quad Rotation Acceleration", &mRotationAcceleration, 0.1f, -100.0f, 100.0f);

                ImGui::ColorEdit4("Quad Color", &mColor.x);
                ImGui::ColorEdit4("Clear Color", &mClearColor.x);

                ImGui::DragFloat("Quad Spacing", &mQuadSpacing, 0.1f, -20.0f, 20.0f);
                ImGui::DragInt("Vertical Quad Count", &mVerticalQuadCount, 1, -100, 100);
                ImGui::DragInt("Horizontal Quad Count", &mHorizontalQuadCount, 1, -100, 100);

                const auto& stats = Renderer2D::GetStatistics();

                ImGui::Text("Renderer2D Statistics:");
                ImGui::Text("\tDraw Calls: %i", stats.DrawCalls);
                ImGui::Text("\tQuads:      %i", stats.QuadCount);
                ImGui::Text("\tVertices:   %i", stats.TotalVertexCount);
                ImGui::Text("\tIndices:    %i", stats.TotalIndexCount);

                ImGui::Text("Delta Time: %fms", ((TimeUnit)Time::GetDeltaTime()).InMilliSeconds());
                ImGui::Text("FPS: %fs", Time::GetFPS().InSeconds());
                ImGui::Text("Average FPS: %fs", Time::GetAverageFPS().InSeconds());
                ImGui::Text("Current Time: %fs", Time::GetTime().InSeconds());
            }
            ImGui::End();

            bool open = true;
            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoScrollbar;
            ImGui::Begin("Viewport", &open, windowFlags);
            {
                static uint32 previousWidth  = ImGui::GetWindowWidth();
                static uint32 previousHeight = ImGui::GetWindowHeight();

                uint32 width = ImGui::GetWindowWidth();
                uint32 height = ImGui::GetWindowHeight();

                uint32 textureID = mFramebuffer->GetColorAttachmentRendererID();
                ImGui::Image((void*)textureID, ImVec2((float32)mFramebuffer->GetInfo().Width, (float32)mFramebuffer->GetInfo().Height));

                if (width != previousWidth || height != previousHeight)
                {
                    mFramebuffer->Resize(width, height);
                    mCameraController.OnResize(width, height);
                }

                previousWidth  = width;
                previousHeight = height;
            }
            ImGui::End();
        }

    private:
        glm::vec3 mPosition   = { 0.0f, 0.0f, 0.0f };
        float32   mRotation   = 0.0f;
        glm::vec2 mScale      = { 1.0f, 1.0f };
        float4    mColor      = { 0.8f, 0.2f, 0.3f, 1.0f };
        float4    mClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

        float32 mPositionAcceleration = 1.0f;
        float32 mRotationAcceleration = 1.0f;

        float32 mQuadSpacing = 1.1f;

        OrthographicCameraController mCameraController;

        Ref<Texture2D> mCosmicLogoTexture;
        Ref<Framebuffer> mFramebuffer;

        int32 mVerticalQuadCount   = 10;
        int32 mHorizontalQuadCount = 10;
    };

    class SandboxApp : public Application
    {
    public:
        SandboxApp()
        {
            CS_PROFILE_FN();

            Init({});
        }

        void OnInit(const ApplicationInitEvent& e)
        {
            CS_PROFILE_FN();

            ModuleSystem::Add<DebugModule>();
        }

        void OnEvent(const Event& e) override
        {
            CS_PROFILE_FN();

            Application::OnEvent(e);

            EventDispatcher dispatcher(e);
            CS_DISPATCH_EVENT(ApplicationInitEvent, OnInit);
        }
    };

    Application* CreateApplication()
    {
        CS_PROFILE_FN();

        return new SandboxApp();
    }

}