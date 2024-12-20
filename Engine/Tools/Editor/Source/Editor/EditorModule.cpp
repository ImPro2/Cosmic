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
#include "UI/ImGuiUtils/ImGuiUtils.hpp"
#include "UI/DockspaceModule.hpp"

namespace Cosmic
{

    void EditorModule::OnInit()
    {
        CS_PROFILE_FN();
        
        mActiveProject = ProjectManager::NewProject();
        mActiveScene   = mActiveProject->GetActiveScene();

        SetWindowTitle();

        mPanels.Init();
        mLayoutManager.Init(FileSystem::GetCurrentWorkingDirectory() / "Engine/Tools/Editor/Assets/EditorLayouts.yaml");

        ModuleSystem::Add<MenubarModule>(MenubarLayout::Default());
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
        CS_DISPATCH_EVENT(FileAddedEvent, OnFileAdded);
        CS_DISPATCH_EVENT(SceneOpenedEvent, OnSceneOpened);
        CS_DISPATCH_EVENT(SceneNewEvent, OnSceneNew);

        if (e.GetType() >= (int16)EEventType::Last)
			mActionManager.OnEditorEvent(e);
    }

    void EditorModule::OnImGuiRender()
    {
        CS_PROFILE_FN();

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

    bool EditorModule::OnFileAdded(const FileAddedEvent& e)
    {
        const File& modifiedPath       = e.GetFile();
        const File& scriptAssemblyFile = NativeScriptEngine::GetScriptAssemblyFile();

        if (scriptAssemblyFile.GetAbsolutePath() == "")
            return false;

        if (modifiedPath.GetAbsolutePath() == scriptAssemblyFile.GetAbsolutePath())
        {
            NativeScriptEngine::ReloadScriptAssembly();
            return true;
        }

        return false;
    }

    bool EditorModule::OnSceneOpened(const SceneOpenedEvent& e)
    {
        NativeScriptEngine::SetActiveScene(e.GetScene());

        return false;
    }

    bool EditorModule::OnSceneNew(const SceneNewEvent& e)
    {
        NativeScriptEngine::SetActiveScene(e.GetScene());

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
        if (!FileSystem::Exists(file))
        {
            CS_LOG_WARN("Attempted to open nonexistent project file {}", file.GetAbsolutePath().GetString().c_str());
            return;
        }

        mActiveProject = ProjectManager::LoadProject(file);

        mActiveScene     = mActiveProject->GetActiveScene();
        mActiveSceneFile = mActiveProject->GetParentPath() / mActiveProject->GetInfo().StartScenePath.GetAbsolutePath();

        SetWindowTitle();

        EventSystem::DeferEvent<ProjectOpenedEvent>(mActiveProject);
        EventSystem::DeferEvent<SceneOpenedEvent>(mActiveScene);
    }

    void EditorModule::NewProject()
    {
        Ref<FileDialogModule> fileDialogModule = ModuleSystem::AddFront<FileDialogModule>();

        fileDialogModule->SetOpenFileCallback("Cosmic Project", { ".cosmic" }, [this](const File& file)
		{
			NewProject(file);
		});
    }

    void EditorModule::NewProject(File file)
    {
        mActiveProject = ProjectManager::NewProject(file);

        mActiveScene     = mActiveProject->GetActiveScene();
        mActiveSceneFile = mActiveProject->GetParentPath() / mActiveProject->GetInfo().StartScenePath.GetAbsolutePath();

        SetWindowTitle();

        EventSystem::DeferEvent<ProjectNewEvent>(mActiveProject);
        EventSystem::DeferEvent<SceneNewEvent>(mActiveScene);
    }

    void EditorModule::SaveScene()
    {
        if (!mActiveSceneFile.GetAbsolutePath().GetString().empty())
        {
            SceneSerializer serializer(mActiveScene);
            serializer.Serialize(mActiveSceneFile);

            EventSystem::DeferEvent<SceneSavedEvent>(mActiveScene);
        }
    }

    void EditorModule::SaveSceneAs()
    {
        Ref<FileDialogModule> fileDialogModule = ModuleSystem::AddFrontDeferred<FileDialogModule>();
        
        fileDialogModule->SetSaveFileCallback("Scene.cscene", { "Cosmic Scene (*.cscene)" }, [this](File file) { SaveSceneAs(file); });
    }

    void EditorModule::SaveSceneAs(File file)
    {
        mActiveSceneFile = file.GetAbsolutePath();
        SceneSerializer serializer(mActiveScene);
        serializer.Serialize(mActiveSceneFile);

        EventSystem::DeferEvent<SceneSavedAsEvent>(mActiveScene);
    }

    void EditorModule::OpenScene()
    {
        {
            SceneSerializer serializer(mActiveScene);
            serializer.Serialize(mActiveSceneFile);
        }

        Ref<FileDialogModule> fileDialogModule = ModuleSystem::AddFrontDeferred<FileDialogModule>();
        fileDialogModule->SetOpenFileCallback("Cosmic Scene", { ".cscene" }, [this](File file) { OpenScene(file); });
    }

    void EditorModule::OpenScene(File file)
    {
        mActiveSceneFile = file.GetAbsolutePath();
        mActiveScene     = CreateRef<Scene>();

        SceneSerializer serializer(mActiveScene);
        serializer.Deserialize(mActiveSceneFile);

        EventSystem::DeferEvent<SceneOpenedEvent>(mActiveScene);
    }

    void EditorModule::NewScene()
    {
        mActiveScene = CreateRef<Scene>();
        EventSystem::DeferEvent<SceneNewEvent>(mActiveScene);
    }

    void EditorModule::SetWindowTitle()
    {
        String projectPath = mActiveProject->GetInfo().ProjectFilePath.GetAbsolutePath();
        String scenePath   = mActiveSceneFile.GetAbsolutePath();

        if (projectPath.empty())
            projectPath = "Unsaved Project";

        if (scenePath.empty())
            scenePath = "Unsaved Scene";

        Application::Get()->GetWindow()->SetTitle(std::format("Cosmic Editor - {} - {}", projectPath, scenePath));
    }

}

