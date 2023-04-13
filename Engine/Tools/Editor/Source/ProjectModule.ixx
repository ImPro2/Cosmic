module;
#include "cspch.hpp"
export module Editor.ProjectModule;

import Cosmic.App.Module;
import Cosmic.Base;
import Cosmic.Time.DeltaTime;
import Cosmic.App.FileSystem;
import Cosmic.App.OS;

import Editor.Project;

namespace Cosmic
{

	export class ProjectModule : public Module
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
