#include "cspch.hpp"
#include "ProjectManager.hpp"
#include "ProjectSerializer.hpp"

namespace Cosmic
{

    Ref<Project> ProjectManager::NewProject()
    {
        sActiveProject = CreateRef<Project>();
        return sActiveProject;
    }

    Ref<Project> ProjectManager::LoadProject(const Path& path)
    {
        ProjectSerializer serializer(sActiveProject);
        serializer.Deserialize(path);
        
        return sActiveProject;
    }
     
    void ProjectManager::SaveActiveProject()
    {
        ProjectSerializer serializer(sActiveProject);
        serializer.Serialize(sActiveProject->GetInfo().ProjectFilePath.GetAbsolutePath());
    }

}
