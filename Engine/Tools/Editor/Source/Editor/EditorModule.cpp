#include "cspch.hpp"
#include "EditorModule.hpp"
#include "App/KeyAndMouseCodes.hpp"

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
#include "Panels/ContentBrowserPanel.hpp"

#include "UI/FileDialog.hpp"
#include "UI/Menubar/MenubarModule.hpp"
#include "UI/ImGuiUtil.hpp"
#include "UI/DockspaceModule.hpp"

namespace Cosmic
{

    void EditorModule::OnInit()
    {
        CS_PROFILE_FN();
        
        mActiveScene = CreateRef<Scene>();
        mPanels.Init();

        ModuleSystem::Add<MenubarModule>(MenubarLayout::Default());

        mLayoutManager.Init();

        ModuleSystem::AddFront<DockspaceModule>();
    }

    void EditorModule::OnShutdown()
    {
        mLayoutManager.Shutdown();

        CS_PROFILE_FN();
    }

    void EditorModule::OnUpdate(Dt dt)
    {
        CS_PROFILE_FN();
    }

    void EditorModule::OnEvent(const IEvent& e)
    {
        CS_PROFILE_FN();

        EventDispatcher dispatcher(e);
        CS_DISPATCH_EVENT(KeyPressEvent, OnKeyPressed);
        CS_DISPATCH_EVENT(FileModifiedEvent, OnFileModified);

        if (e.GetType() >= (int16)EEventType::Last)
			mActionManager.OnEditorEvent(e);
    }

    void EditorModule::OnImGuiRender()
    {
        CS_PROFILE_FN();

        //SetupDockSpace();

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

        if (viewport->Size.x == 0 || viewport->Size.y == 0)
        {
            mSetupDefaultLayout = true;
            return;
        }
        
        ImGui::DockBuilderRemoveNode(dockspaceID);
        ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->Size);

        const Ref<ConsolePanel>&        consolePanel        = mPanels.GetPanel<ConsolePanel>();    
        const Ref<ViewportPanel>&       viewportPanel       = mPanels.GetPanel<ViewportPanel>();
        const Ref<InspectorPanel>&      inspectorPanel      = mPanels.GetPanel<InspectorPanel>();
        const Ref<SceneHierarchyPanel>& sceneHierarchyPanel = mPanels.GetPanel<SceneHierarchyPanel>();
        const Ref<ContentBrowserPanel>& contentBrowserPanel = mPanels.GetPanel<ContentBrowserPanel>();

        ImGuiID dockIdLeft = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Left, 0.4f, nullptr, &dockspaceID);
        ImGuiID dockIdRight = dockspaceID;
        ImGuiID dockIdSceneHierarchy = ImGui::DockBuilderSplitNode(dockIdLeft, ImGuiDir_Up, 0.5f, nullptr, &dockIdLeft);
        ImGuiID dockIdInspector = dockIdLeft;
        ImGuiID dockIdViewport = ImGui::DockBuilderSplitNode(dockIdRight, ImGuiDir_Up, 0.7f, nullptr, &dockIdRight);
        ImGuiID dockIdConsole = dockIdRight;
        ImGuiID dockIdContentBrowser = dockIdConsole;

        ImGui::DockBuilderDockWindow(consolePanel->GetPanelName().c_str(),        dockIdConsole);
        ImGui::DockBuilderDockWindow(viewportPanel->GetPanelName().c_str(),       dockIdViewport);
        ImGui::DockBuilderDockWindow(inspectorPanel->GetPanelName().c_str(),      dockIdInspector);
        ImGui::DockBuilderDockWindow(sceneHierarchyPanel->GetPanelName().c_str(), dockIdSceneHierarchy);
        ImGui::DockBuilderDockWindow(contentBrowserPanel->GetPanelName().c_str(), dockIdContentBrowser);
    
        ImGui::DockBuilderFinish(dockspaceID);
    }

    bool EditorModule::OnKeyPressed(const KeyPressEvent& e)
    {
        bool control = Input::IsKeyPressed(EKeyCode::LeftControl) || Input::IsKeyPressed(EKeyCode::RightControl);
        bool shift   = Input::IsKeyPressed(EKeyCode::LeftShift)   || Input::IsKeyPressed(EKeyCode::RightShift);
        bool alt     = Input::IsKeyPressed(EKeyCode::LeftAlt)     || Input::IsKeyPressed(EKeyCode::RightAlt);

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

            EventSystem::DeferEvent<SceneSavedEvent>(mActiveScene);
        }
    }

    void EditorModule::SaveSceneAs(File file)
    {
        mActiveScenePath = file.GetAbsolutePath();
        SceneSerializer serializer(mActiveScene);
        serializer.Serialize(mActiveScenePath);

        EventSystem::DeferEvent<SceneSavedAsEvent>(mActiveScene);
    }

    void EditorModule::SaveSceneAs()
    {
        Ref<FileDialogModule> fileDialogModule = ModuleSystem::AddFrontDeferred<FileDialogModule>(".");
        
        fileDialogModule->SetSaveFileCallback("Scene.cscene", { "Cosmic Scene (*.cscene)" }, [this](File file) { SaveSceneAs(file); });
    }

    void EditorModule::OpenScene(File file)
    {
        mActiveScenePath = file.GetAbsolutePath();
        mActiveScene = CreateRef<Scene>();

        SceneSerializer serializer(mActiveScene);
        serializer.Deserialize(mActiveScenePath);

        EventSystem::DeferEvent<SceneOpenedEvent>(mActiveScene);
    }

    void EditorModule::OpenScene()
    {
        {
            SceneSerializer serializer(mActiveScene);
            serializer.Serialize(mActiveScenePath);
        }

        Ref<FileDialogModule> fileDialogModule = ModuleSystem::AddFrontDeferred<FileDialogModule>(Path("Engine/Tools/Editor"));
        fileDialogModule->SetOpenFileCallback("Cosmic Scene", { ".cscene" }, [this](File file) { OpenScene(file); });
    }

    void EditorModule::NewScene()
    {
        CS_NOT_IMPLEMENTED();    
    }

}

