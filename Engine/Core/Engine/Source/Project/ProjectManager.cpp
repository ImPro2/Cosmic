#include "cspch.hpp"
#include "ProjectManager.hpp"
#include "ProjectSerializer.hpp"

#include "App/Application.hpp"
#include "Script/NativeScriptEngine.hpp"
#include "ECS/SceneSerializer.hpp"

CS_MODULE_LOG_INFO(Cosmic, Project.ProjectManager);

namespace Cosmic
{

    PersistentRef<ProjectManager> ProjectManager::Init()
    {
        sInstance = CreatePersistentRef<ProjectManager>();

        return sInstance;
    }

    void ProjectManager::Shutdown()
    {
        sInstance.Release();
    }

    Ref<Project> ProjectManager::NewProject()
    {
        sInstance->mActiveProject = CreateRef<Project>();

        CS_LOG_INFO("Created new project");

        return sInstance->mActiveProject;
    }

    Ref<Project> ProjectManager::NewProject(const Path& path)
    {
        ProjectInfo info;
        info.ProjectFilePath = path;

        sInstance->mActiveProject = CreateRef<Project>(info);

        Path projectDir = sInstance->mActiveProject->GetParentPath();
        Path assetsDir  = projectDir / info.AssetsDirectory;
        Path sourceDir  = projectDir / info.SourceDirectory;

        FileSystem::EnsureDirectoryExists(assetsDir);
        FileSystem::EnsureDirectoryExists(sourceDir);

        SaveActiveProject();

        CS_LOG_INFO("Created new project {}", path.GetString().c_str());

        return sInstance->mActiveProject;
    }

    Ref<Project> ProjectManager::LoadProject(const Path& path)
    {
        ProjectSerializer serializer(sInstance->mActiveProject);
        serializer.Deserialize(path);

        ProjectInfo& info = sInstance->mActiveProject->GetInfo();

        Path projectDir         = sInstance->mActiveProject->GetParentPath();
        Path scriptAssemblyPath = projectDir / info.ScriptAssemblyPath.GetAbsolutePath();
        Path startScenePath     = projectDir / info.StartScenePath.GetAbsolutePath();

        if (FileSystem::FileExists(scriptAssemblyPath))
        {
            NativeScriptEngine::LoadScriptAssembly(scriptAssemblyPath);
        }
        else
        {
            if (info.ScriptAssemblyPath.GetAbsolutePath() == "")
                CS_LOG_WARN("Unspecified script assembly path");
            else
				CS_LOG_WARN("Invalid script assembly path {}", scriptAssemblyPath.GetString().c_str());

            info.ScriptAssemblyPath = Path("");
        }

        if (FileSystem::FileExists(startScenePath))
        {
            SceneSerializer serializer(sInstance->mActiveProject->GetActiveScene());
            serializer.Deserialize(startScenePath);
        }
        else
        {
            if (info.StartScenePath.GetAbsolutePath() == "")
                CS_LOG_WARN("Unspecified scene path");
            else
				CS_LOG_WARN("Invalid starting scene path {}", startScenePath.GetString().c_str());

            info.StartScenePath = Path("");
        }

        CS_LOG_INFO("Loaded project {}", info.ProjectFilePath.GetAbsolutePath().GetString().c_str());
        
        return sInstance->mActiveProject;
    }
     
    void ProjectManager::SaveActiveProject(const Path& path)
    {
        if (!path.GetString().empty())
			sInstance->mActiveProject->GetInfo().ProjectFilePath = path;

        ProjectSerializer serializer(sInstance->mActiveProject);
        serializer.Serialize(sInstance->mActiveProject->GetInfo().ProjectFilePath);
    }

}
