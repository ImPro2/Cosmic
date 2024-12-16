#pragma once
#include "Project.hpp"
#include "Memory/SmartPtrs.hpp"

namespace Cosmic
{

	class Application;

    class ProjectManager : public IRefCounted
	{
	public:
        static Ref<Project> NewProject();
        static Ref<Project> LoadProject(const Path& path);
        
		static void SaveActiveProject(const Path& path = "");

	public:
		static Ref<Project> GetActiveProject() { return sInstance->mActiveProject; }

	private:
		static Ref<ProjectManager> Init();
		static void                Shutdown();

	private:
		Ref<Project> mActiveProject;

		inline static Ref<ProjectManager> sInstance;

		friend class Application;
	};

}
