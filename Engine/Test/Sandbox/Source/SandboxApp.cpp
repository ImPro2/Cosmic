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

#include <entt/entt.hpp>

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
import Cosmic.App.FileSystem;
import Cosmic.App.File;
import Cosmic.App.FileSystemEvents;
import Cosmic.ECS.Scene;
import Cosmic.ECS.Entity;
import Cosmic.ECS.Components;

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

            mScene = CreateRef<Scene>();

            Entity entity = mScene->CreateEntity("Circle");
            entity.AddComponent<SpriteRendererComponent>();
            auto& tc1 = entity.GetComponent<TransformComponent>();
            tc1.Translation = { -5.0f, 0.0f, 0.0f };
            mMassMap["Circle"] = 1.0f;

            Entity entity2 = mScene->CreateEntity("Circle2");
            entity2.AddComponent<SpriteRendererComponent>();
            auto& tc2 = entity.GetComponent<TransformComponent>();
            tc1.Translation = { 5.0f, 0.0f, 0.0f };
            mMassMap["Circle2"] = 1.0f;
        }

        void OnUpdate(Dt dt) override
        {
            CS_PROFILE_FN();

            // Update

            mCameraController.OnUpdate();
            Renderer2D::ResetStatistics();

            // Render

            RenderCommand::Clear();
            mFramebuffer->Bind();
            RenderCommand::Clear();

            CalculateGravity();
            
            mScene->OnUpdateEditor(dt, mCameraController.GetCamera());

            mFramebuffer->Unbind();
        }

        void CalculateGravity()
        {
            // F = G * M_1 * M_2 * d^-2

            mScene->ForEachEntity([this](Entity entity1)
            {
                mScene->ForEachEntity([this, &entity1](Entity entity2)
                {
                    float G = 0.0001f;

                    float M1 = mMassMap[entity1.GetComponent<TagComponent>().Tag];
                    float M2 = mMassMap[entity2.GetComponent<TagComponent>().Tag];

                    auto& tc1 = entity1.GetComponent<TransformComponent>();
                    auto& tc2 = entity2.GetComponent<TransformComponent>();

                    glm::vec3 translation1 = tc1.Translation;
                    glm::vec3 translation2 = tc2.Translation;

                    glm::vec3 Distance = translation1 - translation2;

                    glm::vec3 Force = glm::vec3(0.0f);
                    Force.x = (G * M1 * M2) / (Distance.x * Distance.x);
                    Force.y = (G * M1 * M2) / (Distance.y * Distance.y);

                    glm::vec3 Acceleration1 = Force / M1;
                    glm::vec3 Acceleration2 = Force / M2;

                    entity1.GetComponent<TransformComponent>().Translation += Acceleration1;
                    entity2.GetComponent<TransformComponent>().Translation += Acceleration2;
                });
            });
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


            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
            windowFlags |= ImGuiWindowFlags_NoNavInputs;
            windowFlags |= ImGuiWindowFlags_NoScrollbar;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            bool open = true;
            ImGui::Begin("Viewport", &open, windowFlags);
            {
                // BLock events if the panel is selected or hovered.
                bool hovered = ImGui::IsWindowHovered();
                bool focused = ImGui::IsWindowFocused();
                Gui::BlockEvents(!hovered && !focused);

                uint32 fbWidth = mFramebuffer->GetInfo().Width;
                uint32 fbHeight = mFramebuffer->GetInfo().Height;

                uint32 width = ImGui::GetContentRegionAvail().x;
                uint32 height = ImGui::GetContentRegionAvail().y;

                // Check if the framebuffer's size matches the window size

                if (fbWidth != width || fbHeight != height && width > 0.0f && height > 0.0f)
                {
                    mFramebuffer->Resize(width, height);
                    mCameraController.OnResize(width, height);

                    mScene->OnViewportResize(width, height);
                }

                // Render the image

                uint32 textureID = mFramebuffer->GetColorAttachmentRendererID();
                ImGui::Image((void*)textureID, ImVec2((float32)width, (float32)height), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
            }

            ImGui::PopStyleVar(2);
            ImGui::End();
        }

    private:
        OrthographicCameraController mCameraController;

        Ref<Texture2D> mCosmicLogoTexture;
        Ref<Framebuffer> mFramebuffer;

        Ref<Scene> mScene;
        
        UnorderedMap<String, float32> mMassMap;
    };

    class SandboxApp : public Application
    {
    public:
        SandboxApp()
        {
            CS_PROFILE_FN();

            Init({});
        }

        bool OnInit(const ApplicationInitEvent& e)
        {
            CS_PROFILE_FN();

            ModuleSystem::Add<DebugModule>();

            return false;
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