#pragma once
#include "Project.hpp"

namespace Cosmic
{
    
    class ProjectSerializer
	{
	public:
		ProjectSerializer(Ref<Project> project);

	public:
		void Serialize(const Path& path);
		bool Deserialize(const Path& path);

	private:
		Ref<Project> mProject;
	};

}
