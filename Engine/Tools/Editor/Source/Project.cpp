#include "cspch.hpp"
#include "Project.hpp"

namespace Cosmic
{

	void ProjectManager::SetActive(const Ref<Project>& project)
	{
		sActiveProject = project;

		if (!sActiveProject->GetInfo().RootDirectory.empty())
		{
			// TODO:
			FileSystem::CreateDirectory(sActiveProject->GetInfo().AssetsDirectory);
		}
	}

}
