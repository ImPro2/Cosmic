#pragma once

#include "Editor/Event/EditorEvents.hpp"
#include "Base/Base.hpp"
#include "ECS/Scene.hpp"
#include "ECS/Entity.hpp"

namespace Cosmic
{

	struct EditorSceneEvent : public Event
	{
		EditorSceneEvent(const Ref<Scene>& scene)
			: mScene(scene)
		{
		}

		const Ref<Scene>& GetScene() const { return mScene; }
		Ref<Scene>        GetScene() { return mScene; }

	private:
		Ref<Scene> mScene;
	};

	struct EditorSceneSavedEvent : public EditorSceneEvent
	{
		EditorSceneSavedEvent(const Ref<Scene>& scene)
			: EditorSceneEvent(scene)
		{
		}

        CS_EVENT_TYPE(EEditorEvent::SceneSaved);
	};

	struct EditorSceneSavedAsEvent : public EditorSceneEvent
	{
		EditorSceneSavedAsEvent(const Ref<Scene>& scene)
			: EditorSceneEvent(scene)
		{
		}

        CS_EVENT_TYPE(EEditorEvent::SceneSavedAs);
	};

	struct EditorSceneOpenedEvent : public EditorSceneEvent
	{
		EditorSceneOpenedEvent(const Ref<Scene>& scene)
			: EditorSceneEvent(scene)
		{
		}

        CS_EVENT_TYPE(EEditorEvent::SceneOpened);
	};

	struct EditorSceneNewEvent : public EditorSceneEvent
	{
		EditorSceneNewEvent(const Ref<Scene>& scene)
			: EditorSceneEvent(scene)
		{
		}

        CS_EVENT_TYPE(EEditorEvent::SceneNew);
	};

    struct EditorEntityEvent : public EditorSceneEvent
    {
        EditorEntityEvent(Entity entity, const Ref<Scene>& scene)
            : mEntity(entity), EditorSceneEvent(scene)
        {
        }

        Entity GetEntity() { return mEntity; }

    private:
        Entity mEntity;
    };

    struct EditorEntityAddedEvent : public EditorEntityEvent
    {
        EditorEntityAddedEvent(Entity entity, const Ref<Scene>& scene)
            : EditorEntityEvent(entity, scene)
        {
        }

        CS_EVENT_TYPE(EEditorEvent::EntityAdded);
    };

    struct EditorEntityRemovedEvent : public EditorEntityEvent
    {
        EditorEntityRemovedEvent(Entity entity, const Ref<Scene>& scene)
            : EditorEntityEvent(entity, scene)
        {
        }

        CS_EVENT_TYPE(EEditorEvent::EntityRemoved);
    };

}
