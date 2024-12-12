#include "cspch.hpp"
#include "CustomLayoutModule.hpp"

#include <imgui.h>

namespace Cosmic
{

	void CustomLayoutModule::OnImGuiRender()
	{
		ImGui::OpenPopup(sWindowName.c_str());

		static float32 height = 0.0f;

		ImVec2 size;

		size = ImVec2(ImGui::GetMainViewport()->Size.x * 0.35f, height == 0.0f ? ImGui::GetMainViewport()->Size.y * 0.15f : height);

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(size, ImGuiCond_Always);
        
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

		bool cancelled = false;
		bool close     = false;
        
		if (ImGui::BeginPopupModal(sWindowName.c_str(), nullptr, windowFlags))
		{
			ImGui::Text("Enter Layout Name:");
			ImGui::SameLine();

			ImGui::PushID(12);
			close = ImGui::InputText("", mInputBuffer, 256, ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::PopID();

			ImGui::Text("");
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 100.0f);

			cancelled = ImGui::Button("Cancel"); ImGui::SameLine();
			close     = ImGui::Button("Save");

			height = ImGui::GetCursorPos().y + 4.0f;

			ImGui::EndPopup();
		}

		if (close || cancelled)
			Close(cancelled);
	}

	void CustomLayoutModule::OnNameEntered(CustomLayoutNameEnteredCallback callback)
	{
		mCallback = callback;
	}

	void CustomLayoutModule::Close(bool cancelled)
	{
		if (!cancelled)
			mCallback(String(mInputBuffer));

		ModuleSystem::RemoveDeferred<CustomLayoutModule>();
	}

}
