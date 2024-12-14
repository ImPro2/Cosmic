#include "cspch.hpp"
#include "ProjectManager.hpp"
#include "ProjectSerializer.hpp"

namespace Cosmic
{

    Ref<Project> ProjectManager::NewProject()
    {
        ProjectInfo info;
        info.AssetsDirectory = "Assets";

        sActiveProject = CreateRef<Project>(info);
        return sActiveProject;
    }

    Ref<Project> ProjectManager::LoadProject(const Path& path)
    {
        ProjectSerializer serializer(sActiveProject);
        serializer.Deserialize(path);
        
        return sActiveProject;
    }
     
    void ProjectManager::SaveActiveProject(const Path& path)
    {
        if (!path.GetString().empty())
			sActiveProject->GetInfo().ProjectFilePath = path;

        ProjectSerializer serializer(sActiveProject);
        serializer.Serialize(sActiveProject->GetInfo().ProjectFilePath);
    }

}
