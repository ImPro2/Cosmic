#pragma once
#include "Project.hpp"

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
		Vector<Ref<Project>> mProjects;
		Directory mProjectsDirectory = "C:\\Dev\\Cosmic\\Engine\\Test\\Projects";
	};

}
