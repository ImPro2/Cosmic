#pragma once

#include "App/Event/AppEvents.hpp"
#include "Base/Base.hpp"
#include "ECS/Scene.hpp"

namespace Cosmic
{

	struct EditorSceneEvent : public Event
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

	struct EditorSceneSavedEvent : public EditorSceneEvent
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

	struct EditorSceneSavedAsEvent : public EditorSceneEvent
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

	struct EditorSceneOpenedEvent : public EditorSceneEvent
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

	struct EditorSceneNewEvent : public EditorSceneEvent
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
