module;
#include "cspch.hpp"
export module Cosmic.App.EditorEvents;

import Cosmic.App.Events;
import Cosmic.Base;
import Cosmic.ECS.Scene;

namespace Cosmic
{

	export struct EditorSceneEvent : public Event
	{
	public:
		EditorSceneEvent(const Ref<Scene>& scene)
			: mScene(scene)
		{
		}

	public:
		const Ref<Scene>& GetScene() const { return mScene; }
		Ref<Scene>        GetScene() { return mScene; }

	private:
		Ref<Scene> mScene;
	};

	export struct EditorSceneSavedEvent : public EditorSceneEvent
	{
	public:
		EditorSceneSavedEvent(const Ref<Scene>& scene)
			: EditorSceneEvent(scene)
		{
		}

	public:
		       EEventType GetType() const { return EEventType::EditorSceneSaved; }
		static EEventType GetStaticType() { return EEventType::EditorSceneSaved; }
	};

	export struct EditorSceneSavedAsEvent : public EditorSceneEvent
	{
	public:
		EditorSceneSavedAsEvent(const Ref<Scene>& scene)
			: EditorSceneEvent(scene)
		{
		}

	public:
		       EEventType GetType() const { return EEventType::EditorSceneSavedAs; }
		static EEventType GetStaticType() { return EEventType::EditorSceneSavedAs; }
	};

	export struct EditorSceneOpenedEvent : public EditorSceneEvent
	{
	public:
		EditorSceneOpenedEvent(const Ref<Scene>& scene)
			: EditorSceneEvent(scene)
		{
		}

	public:
    		   EEventType GetType() const { return EEventType::EditorSceneOpened; }
		static EEventType GetStaticType() { return EEventType::EditorSceneOpened; }
	};

	export struct EditorSceneNewEvent : public EditorSceneEvent
	{
	public:
		EditorSceneNewEvent(const Ref<Scene>& scene)
			: EditorSceneEvent(scene)
		{
		}

	public:
		       EEventType GetType() const { return EEventType::EditorSceneNew; }
		static EEventType GetStaticType() { return EEventType::EditorSceneNew; }
	};

}
