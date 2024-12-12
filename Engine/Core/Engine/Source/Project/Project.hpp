#pragma once
#include "Base/Base.hpp"
#include "App/FileSystem.hpp"
#include "App/Path.hpp"
#include "App/File.hpp"
#include "Memory/StrongRef.hpp"

namespace Cosmic
{

	struct ProjectInfo
	{
        String ProjectName;
		File ProjectFilePath;
        File StartScenePath;
		Path AssetsDirectory;
	};

	class Project : public IRefCounted
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
	
}
