#include "cspch.hpp"
#include "ProjectManager.hpp"
#include "ProjectSerializer.hpp"

#include "App/Application.hpp"
#include "Script/NativeScriptEngine.hpp"

namespace Cosmic
{

    Ref<ProjectManager> ProjectManager::Init()
    {
        sInstance = CreateRef<ProjectManager>();

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

        NativeScriptEngine::LoadScriptAssembly(sInstance->mActiveProject->GetInfo().ScriptAssemblyPath.GetAbsolutePath());
        
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
