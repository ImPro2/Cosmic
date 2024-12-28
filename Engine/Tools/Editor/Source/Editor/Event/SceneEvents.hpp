#pragma once

#include "Editor/Event/EditorEvents.hpp"
#include "Base/Base.hpp"
#include "ECS/Scene.hpp"
#include "ECS/Entity.hpp"

namespace Cosmic
{

	struct ISceneEvent : public IEvent
	{
		ISceneEvent(const Ref<Scene>& scene)
			: mScene(scene)
		{
		}

		const Ref<Scene>& GetScene() const { return mScene; }
		Ref<Scene>        GetScene() { return mScene; }

	private:
		Ref<Scene> mScene;
	};

	struct SceneSavedEvent : public ISceneEvent
	{
		SceneSavedEvent(const Ref<Scene>& scene)
			: ISceneEvent(scene)
		{
		}

        CS_EVENT_TYPE(EEditorEvent::SceneSaved);
	};

	struct SceneSavedAsEvent : public ISceneEvent
	{
		SceneSavedAsEvent(const Ref<Scene>& scene)
			: ISceneEvent(scene)
		{
		}

        CS_EVENT_TYPE(EEditorEvent::SceneSavedAs);
	};

	struct SceneOpenedEvent : public ISceneEvent
	{
		SceneOpenedEvent(const Ref<Scene>& scene)
			: ISceneEvent(scene)
		{
		}

        CS_EVENT_TYPE(EEditorEvent::SceneOpened);
	};

	struct SceneNewEvent : public ISceneEvent
	{
		SceneNewEvent(const Ref<Scene>& scene)
			: ISceneEvent(scene)
		{
		}

        CS_EVENT_TYPE(EEditorEvent::SceneNew);
	};

	struct ScenePlayEvent : public ISceneEvent
	{
		ScenePlayEvent(const Ref<Scene>& scene)
			: ISceneEvent(scene)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::ScenePlay);
	};

	struct SceneStopEvent : public ISceneEvent
	{
		SceneStopEvent(const Ref<Scene>& scene)
			: ISceneEvent(scene)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::SceneStop);
	};

	struct ScenePauseEvent : public ISceneEvent
	{
		ScenePauseEvent(const Ref<Scene>& scene)
			: ISceneEvent(scene)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::ScenePlay);
	};

	struct SceneResumeEvent : public ISceneEvent
	{
		SceneResumeEvent(const Ref<Scene>& scene)
			: ISceneEvent(scene)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::SceneStop);
	};

}
