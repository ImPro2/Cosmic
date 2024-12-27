#include "cspch.hpp"
#include "PlaybarPanel.hpp"
#include "Editor/Panels/ViewportPanel.hpp"
#include "Editor/Event/SceneEvents.hpp"
#include "Editor/EditorModule.hpp"

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
			case ESceneState::Edit: mEditScene->OnEditorUpdate(dt);  break;
			case ESceneState::Play: mPlayScene->OnRuntimeUpdate(dt); break;
		}
	}

	void PlaybarPanel::OnImGuiRender()
	{
		if (!mOpen)
			return;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar;

		if (ImGui::Begin(mPanelName.c_str(), &mOpen, windowFlags))
		{
			if (ImGui::Button("Play"))
			{
				mSceneState = ESceneState::Play;
				PlayScene();
			}

			if (ImGui::Button("Stop"))
			{
				mSceneState = ESceneState::Edit;
				StopScene();
			}
		}

		ImGui::End();
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
