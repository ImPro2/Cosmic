#include "cspch.hpp"
#include "DockspaceModule.hpp"

#include "Editor/EditorModule.hpp"

#include <imgui.h>

namespace Cosmic
{

	void DockspaceModule::OnInit()
	{

	}

	void DockspaceModule::OnShutdown()
	{

	}

	void DockspaceModule::OnImGuiRender()
	{
		ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGuiWindowFlags windowFlags = 0;// = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        static bool open = true;
        ImGui::Begin(sDockspaceName.c_str(), &open, windowFlags);

        ImGui::PopStyleVar(3);
        ImGuiIO& io = ImGui::GetIO();
        ImGuiID dockspaceID = ImGui::GetID(sDockspaceName.c_str());

        LayoutManager& layoutManager = ModuleSystem::Get<EditorModule>()->GetLayoutManager();

        if (layoutManager.SaveLayout())
        {
            Layout& layout = layoutManager.GetCurrentLayout();
            layout.ConstructFromCurrentLayout();
            layoutManager.SerializeLayouts();
        }

        ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);

        if (layoutManager.SwitchLayout())
        {
            Layout& layout = layoutManager.GetSwitchLayout();
            layout.Load();
        }
        
        ImGui::End();
	}

    void DockspaceModule::LoadLayout()
    {
        LayoutManager& layoutManager = ModuleSystem::Get<EditorModule>()->GetLayoutManager();

    }

}
