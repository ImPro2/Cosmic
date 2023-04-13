module;
#include "cspch.hpp"
#include <imgui.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
module Editor.EditorModule;

CS_MODULE_LOG_INFO(Editor, EditorModule);

import Cosmic.ECS.Entity;
import Cosmic.ECS.SceneSerializer;

namespace Cosmic
{

    void EditorModule::OnInit()
    {
        CS_PROFILE_FN();

        mCosmicLogoTexture = CreateTexture2D("C:/dev/Cosmic/Branding/Logos/Logo.png");

        FramebufferInfo fbInfo = {};
        fbInfo.Width = 1280;
        fbInfo.Height = 720;
        fbInfo.SwapChainTarget = false;
        mFramebuffer = CreateFramebuffer(fbInfo);

        mActiveScene = CreateRef<Scene>();

        mPanels.Init(mFramebuffer, mActiveScene);

        CS_LOG_INFO("Successfully initialized editor.");
    }

    void EditorModule::OnShutdown()
    {
        CS_PROFILE_FN();

        CS_LOG_INFO("Sucessfully shut editor down.");
    }

    void EditorModule::OnUpdate(Dt dt)
    {
        CS_PROFILE_FN();

        Renderer2D::ResetStatistics();
        mFramebuffer->Bind();
        RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.2f, 1.0f });
        RenderCommand::Clear();

        //const OrthographicCamera& camera = mPanels.GetPanel<ViewportPanel>()->GetCameraController().GetCamera();

        mActiveScene->OnUpdate(dt);

        mFramebuffer->Unbind();

        //CS_LOG_INFO("FPS: {}s, Delta Time: {}ms, Current Time: {}s", Time::GetFPS().InSeconds(), Time::GetDeltaTime().InMilliSeconds(), Time::GetTime().InSeconds());
    }

    void EditorModule::OnEvent(const Event& e)
    {
        CS_PROFILE_FN();

        EventDispatcher dispatcher(e);
        CS_DISPATCH_EVENT(KeyPressEvent, OnKeyPressed);
        CS_DISPATCH_EVENT(FileModifiedEvent, OnFileModified);
    }

    void EditorModule::OnImGuiRender()
    {
        CS_PROFILE_FN();

        SetupMenuBar();
        SetupDockSpace();

        ImGui::ShowDemoWindow();
    }

    void EditorModule::SetupDockSpace()
    {
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        static bool open = true;
        ImGui::Begin("Editor Dockspace", &open, windowFlags);

        ImGui::PopStyleVar(3);

        ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyDefault_;
        if (ImGui::BeginTabBar("##MainTabBar", tabBarFlags))
        {


            ImGui::EndTabBar();
        }

        //ImGuiIO& io = ImGui::GetIO();
        //ImGuiID dockspaceID = ImGui::GetID("Editor Dockspace");
        //ImGui::DockSpace(dockspaceID);

        ImGui::End();
    }

    void EditorModule::SetupMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            bool control = Input::IsKeyPressed(EKeyCode::LeftControl) || Input::IsKeyPressed(EKeyCode::RightControl);
            bool shift = Input::IsKeyPressed(EKeyCode::LeftShift) || Input::IsKeyPressed(EKeyCode::RightShift);

            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open Scene", "CTRL+O") || control && Input::IsKeyPressed(EKeyCode::O))
                {
                    OpenScene();
                }
                if (ImGui::MenuItem("Save Scene", "CTRL+S") || control && !shift && Input::IsKeyPressed(EKeyCode::S))
                {
                    SaveScene();
                }
                if (ImGui::MenuItem("Save Scene As", "CTRL+SHIFT+S") || control && shift && Input::IsKeyPressed(EKeyCode::S))
                {
                    SaveSceneAs();
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Show All", ""))
                {
                    mPanels.ShowAll();
                }

                for (Panel* panel : mPanels.GetPanels())
                {
                    ImGui::MenuItem(panel->GetPanelName().c_str(), "", panel->IsOpenPtr());
                }

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    bool EditorModule::OnKeyPressed(const KeyPressEvent& e)
    {
        bool control = Input::IsKeyPressed(EKeyCode::LeftControl) || Input::IsKeyPressed(EKeyCode::RightControl);
        bool shift   = Input::IsKeyPressed(EKeyCode::LeftShift)   || Input::IsKeyPressed(EKeyCode::RightShift);
        bool alt     = Input::IsKeyPressed(EKeyCode::LeftAlt)     || Input::IsKeyPressed(EKeyCode::RightAlt);

        switch (e.GetKeyCode())
        {
            case EKeyCode::S:
            {
                if (control)
                {
                    if (shift)
                    {
                        SaveSceneAs();
                    }
                    else
                    {
                        SaveScene();
                    }
                }
                break;
            }
            case EKeyCode::O:
            {
                if (control)
                {
                    OpenScene();
                }
                break;
            }
            case EKeyCode::N:
            {
                if (control)
                {
                    NewScene();
                }
                break;
            }
            case EKeyCode::D:
            {
                if (control)
                {
                    // Duplicate entity
                }
            }
            case EKeyCode::X:
            {
                if (control)
                {
                    // Remove entity
                }
            }
            case EKeyCode::Q: break;
            case EKeyCode::W: break;
            case EKeyCode::E: break;
            case EKeyCode::R: break;
        }

        return false;
    }

    bool EditorModule::OnFileModified(const FileModifiedEvent& e)
    {
        const String modified = e.GetFile().GetNameAndExtension();
        const String& scriptAssemblyPath = Application::Get()->GetInfo().ScriptAssemblyPath;

        if (modified == scriptAssemblyPath)
        {
            return true;
        }
        return false;
    }

    void EditorModule::SaveScene()
    {
        if (!mActiveScenePath.empty())
        {
            SceneSerializer serializer(mActiveScene);
            serializer.Serialize(mActiveScenePath);

            Application::Get()->OnEvent(EditorSceneSavedEvent(mActiveScene));
        }
    }

    void EditorModule::SaveSceneAs()
    {
        String newPath = OS::SaveFileDialog("");

        if (!newPath.empty())
        {
            mActiveScenePath = newPath;
            SceneSerializer serializer(mActiveScene);
            serializer.Serialize(mActiveScenePath);

            Application::Get()->OnEvent(EditorSceneSavedAsEvent(mActiveScene));
        }
    }

    void EditorModule::OpenScene()
    {
        {
            SceneSerializer serializer(mActiveScene);
            serializer.Serialize(mActiveScenePath);
        }

        String newPath = OS::OpenFileDialog("Cosmic Scene (*.cscene)\0*.cscene\0");
        
        if (!newPath.empty())
        {
            mActiveScenePath = newPath;
            mActiveScene = CreateRef<Scene>();
            
            SceneSerializer serializer(mActiveScene);
            serializer.Deserialize(mActiveScenePath);

            Application::Get()->OnEvent(EditorSceneOpenedEvent(mActiveScene));
        }
    }

    void EditorModule::NewScene()
    {
    
    }

}