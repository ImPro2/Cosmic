#pragma once
#include "Base/Base.hpp"
#include "App/FileSystem.hpp"
#include "App/File.hpp"

namespace Cosmic
{

	struct ProjectInfo
	{
		Directory      RootDirectory;
		String         ProjectFilePath;
		Directory      AssetsDirectory;
		Vector<String> ScenePaths;
	};

	class Project
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

	class ProjectSerializer
	{
	public:
		ProjectSerializer(const Ref<Project>& project);

	public:
		void Serialize();
		void Deserialize();

	private:
		Ref<Project> mProject;
	};

	class ProjectManager
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
