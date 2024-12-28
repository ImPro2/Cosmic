#pragma once

#include "Editor/Panels/Panels.hpp"
#include "Editor/Event/SceneEvents.hpp"

#include "ECS/Scene.hpp"

namespace Cosmic
{

	enum class ESceneState
	{
		Edit, Play, Pause
	};

	class PlaybarPanel : public IPanel
	{
	public:
		PlaybarPanel();

	public:
		void OnInit() override;
		void OnUpdate(Dt dt) override;
		void OnImGuiRender() override;
		void OnEvent(const IEvent& e) override;

	public:
		void PlayScene();
		void StopScene();

	public:
		ESceneState GetSceneState() const { return mSceneState;                      }
		bool        IsPlaying()     const { return mSceneState != ESceneState::Edit; }

		Ref<Scene> GetEditScene() { return mEditScene; }
		Ref<Scene> GetPlayScene() { return mPlayScene; }

	private:
		bool OnSceneOpened(const SceneOpenedEvent& e);
		bool OnSceneNew(const SceneNewEvent& e);

	private:
		Ref<Scene> mEditScene;
		Ref<Scene> mPlayScene;

		ESceneState mSceneState = ESceneState::Edit;
	};

}
