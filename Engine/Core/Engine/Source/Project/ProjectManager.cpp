#include "cspch.hpp"
#include "ProjectManager.hpp"
#include "ProjectSerializer.hpp"

#include "App/Application.hpp"
#include "Script/NativeScriptEngine.hpp"

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
        ProjectInfo info;
        info.AssetsDirectory = "Assets";

        sInstance->mActiveProject = CreateRef<Project>(info);
        return sInstance->mActiveProject;
    }

    Ref<Project> ProjectManager::LoadProject(const Path& path)
    {
        ProjectSerializer serializer(sInstance->mActiveProject);
        serializer.Deserialize(path);

        const Ref<Project>& project = sInstance->mActiveProject;

        NativeScriptEngine::LoadScriptAssembly(project->GetParentPath() / project->GetInfo().ScriptAssemblyPath.GetAbsolutePath());
        
        return project;
    }
     
    void ProjectManager::SaveActiveProject(const Path& path)
    {
        if (!path.GetString().empty())
			sInstance->mActiveProject->GetInfo().ProjectFilePath = path;

        ProjectSerializer serializer(sInstance->mActiveProject);
        serializer.Serialize(sInstance->mActiveProject->GetInfo().ProjectFilePath);
    }

}
