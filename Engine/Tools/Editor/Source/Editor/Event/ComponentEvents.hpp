#pragma once
#include "EditorEvents.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Components.hpp"

namespace Cosmic
{

	struct IComponentEvent : public IEvent
	{
		IComponentEvent(Entity entity, IComponent* component)
			: mEntity(entity), mComponent(component)
		{
		}

		Entity      GetEntity()       const { return mEntity;    }
		IComponent* GetComponentPtr() const { return mComponent; }

		template<class T>
		T GetComponent()
		{
			if (mComponent->GetType() == T::GetStaticType())
				return *static_cast<T*>(mComponent);
		}

	private:
		Entity      mEntity;
		IComponent* mComponent;
	};

	struct ComponentAddedEvent : public IComponentEvent
	{
		ComponentAddedEvent(Entity entity, IComponent* component)
			: IComponentEvent(entity, component)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::ComponentAdded);
	};

	struct ComponentRemovedEvent : public IComponentEvent
	{
		ComponentRemovedEvent(Entity entity, IComponent* component)
			: IComponentEvent(entity, component)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::ComponentRemoved);
	};

	struct ComponentModifiedEvent : public IComponentEvent
	{
		ComponentModifiedEvent(Entity entity, IComponent* component)
			: IComponentEvent(entity, component)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::ComponentModified);
	};

}
