#pragma once
#include "Project/Project.hpp"

#include "App/Module.hpp"
#include "Base/Base.hpp"
#include "Time/DeltaTime.hpp"
#include "App/FileSystem.hpp"
#include "App/OS.hpp"

namespace Cosmic
{

	class ProjectModule : public Module
	{
	public:
		void OnInit() override;
		void OnUpdate(Dt dt) override;
		void OnImGuiRender() override;

	private:
		Path mProjectsDirectory;
	};

}
