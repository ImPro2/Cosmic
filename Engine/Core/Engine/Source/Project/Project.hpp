#pragma once
#include "Base/Base.hpp"
#include "App/FileSystem.hpp"
#include "App/Path.hpp"
#include "App/File.hpp"
#include "ECS/Scene.hpp"
#include "Memory/Memory.hpp"

namespace Cosmic
{

	struct ProjectInfo
	{
		ProjectInfo()                   = default;
		ProjectInfo(const ProjectInfo&) = default;

		// Only absolute path, other paths are relative to the parent directory of ProjectFilePath
		File ProjectFilePath;
		File ScriptAssemblyPath;
		File StartScenePath;

		Path BinaryDirectory = "bin";
		Path AssetsDirectory = "Assets";
		Path SourceDirectory = "Source";
	};

	class Project : public IRefCounted
	{
	public:
		Project() = default;
		Project(const ProjectInfo& info) : mInfo(info) {}

	public:
		const ProjectInfo& GetInfo() const { return mInfo; }
		ProjectInfo&       GetInfo()       { return mInfo; }

		Ref<Scene>&       GetActiveScene()       { return mActiveScene; }
		const Ref<Scene>& GetActiveScene() const { return mActiveScene; }

		String GetName() const
		{
			return mInfo.ProjectFilePath.GetAbsolutePath().GetBase();
		}

		Path GetParentPath() const
		{
			return FileSystem::GetParentDirectory(mInfo.ProjectFilePath);
		}

	public:
		void SetActiveScene(const Ref<Scene>& scene) { mActiveScene = scene; }

	private:
		ProjectInfo mInfo;
		Ref<Scene>  mActiveScene;
	};
	
}
