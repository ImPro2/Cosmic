#pragma once
#include "IEditorAction.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Components.hpp"

namespace Cosmic
{

	class IComponentAction : public IEditorAction
	{
	public:
		IComponentAction(Entity entity, IComponent* component)
			: mEntity(entity), mComponent(component)
		{
		}

	protected:
		Entity      mEntity;
		IComponent* mComponent;
	};

	class ComponentAddedAction : public IComponentAction
	{
	public:
		ComponentAddedAction(Entity entity, IComponent* component)
			: IComponentAction(entity, component)
		{
		}

	public:
		virtual void Undo() override;
		virtual void Redo() override;
	};

	class ComponentRemovedAction : public IComponentAction
	{
	public:
		ComponentRemovedAction(Entity entity, IComponent* component)
			: IComponentAction(entity, component)
		{
		}

	public:
		virtual void Undo() override;
		virtual void Redo() override;
	};

	class ComponentModifedAction : public IComponentAction
	{
	public:
		ComponentModifedAction(Entity entity, IComponent* component)
			: IComponentAction(entity, component)
		{
		}

	public:
		virtual void Undo() override;
		virtual void Redo() override;
	};

}
