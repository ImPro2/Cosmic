#include "cspch.hpp"
#include "EditorModule.hpp"
#include "App/KeyAndMouseCodes.hpp"

#include "Project/ProjectSerializer.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <filesystem>

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
        mLayoutManager.Init(FileSystem::GetCurrentWorkingDirectory() / "Engine/Tools/Editor/Assets/EditorLayouts.yaml");

        ModuleSystem::Add<MenubarModule>(MenubarLayout::Default());
        ModuleSystem::AddFront<DockspaceModule>();

        NewProject();
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

    void EditorModule::SaveProject()
    {
        const ProjectInfo& info = mActiveProject->GetInfo();

        if (!FileSystem::Exists(info.ProjectFilePath))
        {
            SaveProjectAs();
            return;
        }

        ProjectManager::SaveActiveProject(info.ProjectFilePath);
        SetWindowTitle();

        EventSystem::DeferEvent<ProjectSavedEvent>(mActiveProject);
    }

    void EditorModule::SaveProjectAs()
    {
        auto fileDialogModule = ModuleSystem::AddFront<FileDialogModule>();

        fileDialogModule->SetSaveFileCallback("NewProject.cosmic", { "Cosmic Project (*.cosmic)" }, [this](File saveFile)
		{
			SaveProjectAs(saveFile);
		});
    }

    void EditorModule::SaveProjectAs(File file)
    {
        ProjectManager::SaveActiveProject(file);
        SetWindowTitle();

        EventSystem::DeferEvent<ProjectSavedAsEvent>(mActiveProject);
    }

    void EditorModule::OpenProject()
    {
        auto fileDialogModule = ModuleSystem::AddFront<FileDialogModule>();

        fileDialogModule->SetOpenFileCallback("Cosmic Project", { ".cosmic" }, [this](File file)
		{
			OpenProject(file);
		});
    }

    void EditorModule::OpenProject(File file)
    {
        mActiveProject = ProjectManager::LoadProject(file);
        SetWindowTitle();

        EventSystem::DeferEvent<ProjectOpenedEvent>(mActiveProject);
    }

    void EditorModule::NewProject()
    {
        mActiveProject = ProjectManager::NewProject();
        SetWindowTitle();

        EventSystem::DeferEvent<ProjectNewEvent>(mActiveProject);
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
        Ref<FileDialogModule> fileDialogModule = ModuleSystem::AddFrontDeferred<FileDialogModule>();
        
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

        Ref<FileDialogModule> fileDialogModule = ModuleSystem::AddFrontDeferred<FileDialogModule>();
        fileDialogModule->SetOpenFileCallback("Cosmic Scene", { ".cscene" }, [this](File file) { OpenScene(file); });
    }

    void EditorModule::NewScene()
    {
        CS_NOT_IMPLEMENTED();    
    }

    void EditorModule::SetWindowTitle()
    {
        String prjPath = mActiveProject->GetInfo().ProjectFilePath.GetAbsolutePath();

        if (prjPath.empty())
            prjPath = "Unsaved Project";

        Application::Get()->GetWindow()->SetTitle(std::format("Cosmic Editor - {}", prjPath));
    }

}

