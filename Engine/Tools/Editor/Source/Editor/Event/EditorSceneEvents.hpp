#pragma once

#include "Editor/Event/EditorEvents.hpp"
#include "Base/Base.hpp"
#include "ECS/Scene.hpp"
#include "ECS/Entity.hpp"

namespace Cosmic
{

	struct IEditorSceneEvent : public IEvent
	{
		IEditorSceneEvent(const Ref<Scene>& scene)
			: mScene(scene)
		{
		}

		const Ref<Scene>& GetScene() const { return mScene; }
		Ref<Scene>        GetScene() { return mScene; }

	private:
		Ref<Scene> mScene;
	};

	struct EditorSceneSavedEvent : public IEditorSceneEvent
	{
		EditorSceneSavedEvent(const Ref<Scene>& scene)
			: IEditorSceneEvent(scene)
		{
		}

        CS_EVENT_TYPE(EEditorEvent::SceneSaved);
	};

	struct EditorSceneSavedAsEvent : public IEditorSceneEvent
	{
		EditorSceneSavedAsEvent(const Ref<Scene>& scene)
			: IEditorSceneEvent(scene)
		{
		}

        CS_EVENT_TYPE(EEditorEvent::SceneSavedAs);
	};

	struct EditorSceneOpenedEvent : public IEditorSceneEvent
	{
		EditorSceneOpenedEvent(const Ref<Scene>& scene)
			: IEditorSceneEvent(scene)
		{
		}

        CS_EVENT_TYPE(EEditorEvent::SceneOpened);
	};

	struct EditorSceneNewEvent : public IEditorSceneEvent
	{
		EditorSceneNewEvent(const Ref<Scene>& scene)
			: IEditorSceneEvent(scene)
		{
		}

        CS_EVENT_TYPE(EEditorEvent::SceneNew);
	};

    struct IEditorEntityEvent : public IEditorSceneEvent
    {
        IEditorEntityEvent(const Vector<Entity>& entities, const Ref<Scene>& scene)
            : mEntities(entities), IEditorSceneEvent(scene)
        {
        }

        const Vector<Entity>& GetEntities() const { return mEntities; }
		Vector<Entity>        GetEntities()       { return mEntities; }

    private:
        Vector<Entity> mEntities;
    };

    struct EditorEntityAddedEvent : public IEditorEntityEvent
    {
        EditorEntityAddedEvent(const Vector<Entity>& entities, const Ref<Scene>& scene)
            : IEditorEntityEvent(entities, scene)
        {
        }

        CS_EVENT_TYPE(EEditorEvent::EntityAdded);
    };

    struct EditorEntityRemovedEvent : public IEditorEntityEvent
    {
		// Copy entities
        EditorEntityRemovedEvent(Vector<Entity> entities, const Ref<Scene>& scene)
            : IEditorEntityEvent(entities, scene)
        {
        }

        CS_EVENT_TYPE(EEditorEvent::EntityRemoved);
    };

}
