module;
#include "cspch.hpp"
module Editor.Project;

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
