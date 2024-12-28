#include "cspch.hpp"
#include "PlaybarPanel.hpp"
#include "Editor/Panels/ViewportPanel.hpp"
#include "Editor/Event/SceneEvents.hpp"
#include "Editor/EditorModule.hpp"

#include "UI/ImGuiUtils/DrawTypes.hpp"

#include <IconsFontAwesome6.h>

namespace Cosmic
{

	PlaybarPanel::PlaybarPanel()
		: IPanel("Playbar")
	{
	}

	void PlaybarPanel::OnInit()
	{
		mEditScene = ModuleSystem::Get<EditorModule>()->GetActiveScene();
	}

	void PlaybarPanel::OnUpdate(Dt dt)
	{
		switch (mSceneState)
		{
			case ESceneState::Edit:  mEditScene->OnEditorUpdate(dt);        break;
			case ESceneState::Play:  mPlayScene->OnRuntimeUpdate(dt);       break;
			case ESceneState::Pause: mPlayScene->OnRuntimeUpdate(dt, true); break;
		}
	}

	void PlaybarPanel::OnImGuiRender()
	{
		if (!mOpen)
			return;

		float32 lineHeight = ImGuiUtils::GetLineHeight();
		ImVec2  windowSize = { 200.0f, lineHeight };

		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0.0f, lineHeight + 2.0f * 4.0f));

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize;

		ImGui::SetNextWindowSize(windowSize);
		ImGui::SetNextWindowSizeConstraints(windowSize, windowSize);

		if (ImGui::Begin(mPanelName.c_str(), &mOpen, windowFlags))
		{
			static const char* playIcon   = ICON_FA_PLAY;
			static const char* stopIcon   = ICON_FA_SQUARE;
			static const char* pauseIcon  = ICON_FA_PAUSE;
			static const char* resumeIcon = ICON_FA_PAUSE;

			static const char* playStopIcon    = playIcon;
			static const char* pauseResumeIcon = pauseIcon;

			static const ImVec4 playButtonColor = ImVec4(0.1f, 0.9f, 0.2f, 1.0f);
			static const ImVec4 stopButtonColor = ImVec4(1.0f, 0.1f, 0.3f, 1.0f);
			static const ImVec4 pauseResumeButtonColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

			static ImVec4 playStopButtonColor = playButtonColor;

			ImVec2 buttonSize = { lineHeight, lineHeight };

			// Center items
			ImGuiStyle& style = ImGui::GetStyle();

			float32 offset = 0.5f * (ImGui::GetContentRegionAvailWidth() - style.ItemSpacing.x - 2.0f * buttonSize.x + style.FramePadding.x);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);

			ImGui::PushStyleColor(ImGuiCol_Text, playStopButtonColor);

			bool pushPlayStopButtonActiveColor    = mSceneState == ESceneState::Play || mSceneState == ESceneState::Pause;
			bool pushPauseResumeButtonActiveColor = mSceneState == ESceneState::Pause;

			if (pushPlayStopButtonActiveColor)
				ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));

			if (ImGui::Button(playStopIcon, buttonSize))
			{
				if (mSceneState == ESceneState::Edit)
				{
					mSceneState         = ESceneState::Play;
					playStopIcon        = stopIcon;
					playStopButtonColor = stopButtonColor;

					PlayScene();
				}
				else if (mSceneState == ESceneState::Play || mSceneState == ESceneState::Pause)
				{
					mSceneState         = ESceneState::Edit;
					playStopIcon        = playIcon;
					playStopButtonColor = playButtonColor;

					StopScene();
				}
			}

			if (pushPlayStopButtonActiveColor)
				ImGui::PopStyleColor();

			ImGui::PopStyleColor();

			ImGuiUtils::Tooltip(mSceneState == ESceneState::Edit ? "Play Scene" : "Stop Scene");

			ImGui::SameLine();

			if (mSceneState == ESceneState::Edit)
				ImGui::BeginDisabled();

			ImGui::PushStyleColor(ImGuiCol_Text, pauseResumeButtonColor);

			if (pushPauseResumeButtonActiveColor)
				ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));

			if (ImGui::Button(pauseResumeIcon, buttonSize))
			{
				if (mSceneState == ESceneState::Play)
				{
					pauseResumeIcon = resumeIcon;

					PauseScene();
				}
				else if (mSceneState == ESceneState::Pause)
				{
					pauseResumeIcon = pauseIcon;

					ResumeScene();
				}
			}

			if (pushPauseResumeButtonActiveColor)
				ImGui::PopStyleColor();

			ImGui::PopStyleColor();

			if (mSceneState == ESceneState::Edit)
				ImGui::EndDisabled();
			else
				ImGuiUtils::Tooltip(mSceneState == ESceneState::Pause ? "Resume Scene" : "Pause Scene");
		}

		ImGui::End();
		ImGui::PopStyleVar(5);
	}

	void PlaybarPanel::OnEvent(const IEvent& e)
	{
		EventDispatcher dispatcher(e);
		CS_DISPATCH_EVENT(SceneOpenedEvent, OnSceneOpened);
		CS_DISPATCH_EVENT(SceneNewEvent,    OnSceneNew);
	}

	void PlaybarPanel::PlayScene()
	{
		mSceneState = ESceneState::Play;
		mPlayScene = mEditScene->Copy();
		NativeScriptEngine::SetActiveScene(mPlayScene);

		mPlayScene->OnRuntimeStart();

		EventSystem::DeferEvent<ScenePlayEvent>(mPlayScene);
	}

	void PlaybarPanel::StopScene()
	{
		mSceneState = ESceneState::Edit;
		mPlayScene->OnRuntimeStop();
		mPlayScene.Release();
		NativeScriptEngine::SetActiveScene(mEditScene);

		EventSystem::DeferEvent<SceneStopEvent>(mEditScene);
	}

	void PlaybarPanel::PauseScene()
	{
		mSceneState = ESceneState::Pause;

		EventSystem::DeferEvent<ScenePauseEvent>(mPlayScene);
	}

	void PlaybarPanel::ResumeScene()
	{
		mSceneState = ESceneState::Play;

		EventSystem::DeferEvent<SceneResumeEvent>(mPlayScene);
	}

	bool PlaybarPanel::OnSceneOpened(const SceneOpenedEvent& e)
	{
		mEditScene = e.GetScene();
		return false;
	}

	bool PlaybarPanel::OnSceneNew(const SceneNewEvent& e)
	{
		mEditScene = e.GetScene();
		return false;
	}

}
