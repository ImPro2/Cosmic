#pragma once
#include "SceneEvents.hpp"
#include "Memory/Memory.hpp"

namespace Cosmic
{

    struct IEntityEvent : public ISceneEvent
    {
        IEntityEvent(const Vector<Entity>& entities, const Ref<Scene>& scene)
            : mEntities(entities), ISceneEvent(scene)
        {
        }

        const Vector<Entity>& GetEntities() const { return mEntities; }
        Vector<Entity>        GetEntities() { return mEntities; }

    private:
        Vector<Entity> mEntities;
    };

    struct EntityAddedEvent : public IEntityEvent
    {
        EntityAddedEvent(const Vector<Entity>& entities, const Ref<Scene>& scene)
            : IEntityEvent(entities, scene)
        {
        }

        CS_EVENT_TYPE(EEditorEvent::EntityAdded);
    };

    struct EntityRemovedEvent : public IEntityEvent
    {
        // Copy entities
        EntityRemovedEvent(Vector<Entity> entities, const Ref<Scene>& scene)
            : IEntityEvent(entities, scene)
        {
        }

        CS_EVENT_TYPE(EEditorEvent::EntityRemoved);
    };

}
