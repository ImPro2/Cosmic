#include "cspch.hpp"
#include "EditorModule.hpp"
#include "App/Event/Events.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

CS_MODULE_LOG_INFO(Editor, EditorModule);

#include "ECS/Entity.hpp"
#include "ECS/SceneSerializer.hpp"

#include "Panels/ConsolePanel.hpp"
#include "Panels/ViewportPanel.hpp"
#include "Panels/InspectorPanel.hpp"
#include "Panels/SceneHierarchyPanel.hpp"

#include "UI/FileDialog.hpp"

namespace Cosmic
{

    void EditorModule::OnInit()
    {
        CS_PROFILE_FN();

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

        if (mShowDemoWindow)
            ImGui::ShowDemoWindow(&mShowDemoWindow);
    }

    void EditorModule::SetupDockSpace()
    {
        ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGuiWindowFlags windowFlags = 0;// = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
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
        ImGuiIO& io = ImGui::GetIO();
        ImGuiID dockspaceID = ImGui::GetID("Editor Dockspace");
        ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
        
        if (mSetupDefaultLayout)
        {
            mSetupDefaultLayout = false;
            SetupDefaultDockLayout();
        }

        ImGui::End();
    }

    void EditorModule::SetupDefaultDockLayout()
    {
        ImGuiID dockspaceID = ImGui::GetID("Editor Dockspace");
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        
        ImGui::DockBuilderRemoveNode(dockspaceID);
        ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->Size);

        ConsolePanel*        consolePanel        = mPanels.GetPanel<ConsolePanel>();    
        ViewportPanel*       viewportPanel       = mPanels.GetPanel<ViewportPanel>();
        InspectorPanel*      inspectorPanel      = mPanels.GetPanel<InspectorPanel>();
        SceneHierarchyPanel* sceneHierarchyPanel = mPanels.GetPanel<SceneHierarchyPanel>();

        ImGuiID dockIdLeft = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Left, 0.4f, nullptr, &dockspaceID);
        ImGuiID dockIdRight = dockspaceID;
        ImGuiID dockIdSceneHierarchy = ImGui::DockBuilderSplitNode(dockIdLeft, ImGuiDir_Up, 0.5f, nullptr, &dockIdLeft);
        ImGuiID dockIdInspector = dockIdLeft;
        ImGuiID dockIdViewport = ImGui::DockBuilderSplitNode(dockIdRight, ImGuiDir_Up, 0.7f, nullptr, &dockIdRight);
        ImGuiID dockIdConsole = dockIdRight;

        ImGui::DockBuilderDockWindow(consolePanel->GetPanelName().c_str(),        dockIdConsole);
        ImGui::DockBuilderDockWindow(viewportPanel->GetPanelName().c_str(),       dockIdViewport);
        ImGui::DockBuilderDockWindow(inspectorPanel->GetPanelName().c_str(),      dockIdInspector);
        ImGui::DockBuilderDockWindow(sceneHierarchyPanel->GetPanelName().c_str(), dockIdSceneHierarchy);
    
        ImGui::DockBuilderFinish(dockspaceID);
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
                if (ImGui::BeginMenu("Panels"))
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

                ImGui::MenuItem("Show ImGui Demo Window", "", &mShowDemoWindow);

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

            EventSystem::AddEvent(new EditorSceneSavedEvent(mActiveScene));
        }
    }

    void EditorModule::SaveSceneAs()
    {
        FileDialogModule* fileDialogModule = ModuleSystem::AddFrontDeferred<FileDialogModule>(".");
        
        fileDialogModule->SetSaveFileCallback("Scene.cscene", { "Cosmic Scene (*.cscene)" }, [this](File file) {
            mActiveScenePath = file.GetAbsolutePath();
            SceneSerializer serializer(mActiveScene);
            serializer.Serialize(mActiveScenePath);

            EventSystem::AddEvent(new EditorSceneSavedAsEvent(mActiveScene));
        });
    }

    void EditorModule::OpenScene()
    {
        {
            SceneSerializer serializer(mActiveScene);
            serializer.Serialize(mActiveScenePath);
        }

        FileDialogModule* fileDialogModule = ModuleSystem::AddFrontDeferred<FileDialogModule>(Path("Engine/Tools/Editor"));

        fileDialogModule->SetOpenFileCallback("Cosmic Scene", { ".cscene" }, [this](File file) { 
            mActiveScenePath = file.GetAbsolutePath();
            mActiveScene = CreateRef<Scene>();

            SceneSerializer serializer(mActiveScene);
            serializer.Deserialize(mActiveScenePath);

            EventSystem::AddEvent(new EditorSceneOpenedEvent(mActiveScene));
        });
    }

    void EditorModule::NewScene()
    {
        CS_NOT_IMPLEMENTED();    
    }

}
