#pragma once
#include "Project.hpp"
#include "Memory/SmartPtrs.hpp"

namespace Cosmic
{

    class ProjectManager
	{
	public:
        static Ref<Project> NewProject();
        static Ref<Project> LoadProject(const Path& path);
        
		static void SaveActiveProject();

		static Ref<Project> GetActiveProject() { return sActiveProject; }

	private:
		inline static Ref<Project> sActiveProject;
		//inline static Path sProjectsDirectory;
	};

}
