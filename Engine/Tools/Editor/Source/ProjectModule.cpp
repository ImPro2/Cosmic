module;
#include "cspch.hpp"
#include <imgui.h>
#include <filesystem>
module Editor.ProjectModule;

namespace Cosmic
{

	void ProjectModule::OnInit()
	{
		auto dirs = FileSystem::GetAllFilesAndDirectoriesInDirectory(mProjectsDirectory).dirctories;
		for (auto& dir : dirs)
		{
			if (!dir.empty())
			{
				if (std::filesystem::exists(std::filesystem::path(mProjectsDirectory) / dir / (dir + ".cproj")))
				{
				}
			}
		}
	}

	void ProjectModule::OnImGuiRender()
	{
		ImGui::Begin("");
		if (ImGui::Button("New Project..."))
		{
			ImGui::BeginChild("##NewProject", ImVec2(0, 0), true);
			
			String directory = mProjectsDirectory;
			if (ImGui::Button("Projects Directory"))
				directory = OS::OpenFileDialog("");

			char buf[256];
			memset(buf, 0, sizeof(buf));
			ImGui::InputText("Project Name", buf, sizeof(buf));

			ProjectInfo info = {};
			info.RootDirectory = directory;
			info.ProjectFilePath = directory + '/' + String(buf);

			//mProjects.emplace_back(info);

			ImGui::EndChild();
		}
		ImGui::End();
	}


}
