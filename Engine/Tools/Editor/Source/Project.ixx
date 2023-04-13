module;
#include "cspch.hpp"
export module Editor.Project;

import Cosmic.Base;
import Cosmic.App.FileSystem;
import Cosmic.App.File;

namespace Cosmic
{

	export struct ProjectInfo
	{
		Directory      RootDirectory;
		String         ProjectFilePath;
		Directory      AssetsDirectory;
		Vector<String> ScenePaths;
	};

	export class Project
	{
	public:
		Project() = default;
		Project(const ProjectInfo& info) : mInfo(info) {}

	public:
		const ProjectInfo& GetInfo() const { return mInfo; }
		ProjectInfo&       GetInfo()       { return mInfo; }

	private:
		ProjectInfo mInfo;
	};

	export class ProjectSerializer
	{
	public:
		ProjectSerializer(const Ref<Project>& project);

	public:
		void Serialize();
		void Deserialize();

	private:
		Ref<Project> mProject;
	};

	export class ProjectManager
	{
	public:
		static void SetActive(const Ref<Project>& project);
		static void SetProjectsDirectory(const Directory& dir) { sProjectsDirectory = dir; }
		static Ref<Project>& GetActive() { return sActiveProject; }

	private:
		inline static Ref<Project> sActiveProject;
		inline static Directory sProjectsDirectory;
	};

}
