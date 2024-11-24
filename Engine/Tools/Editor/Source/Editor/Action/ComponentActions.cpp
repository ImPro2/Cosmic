#include "cspch.hpp"
#include "ComponentActions.hpp"

namespace Cosmic
{

	void ComponentAddedAction::Undo()
	{
		mEntity.RemoveComponent(mComponent->GetType());
	}
	
	void ComponentAddedAction::Redo()
	{
		mEntity.AddComponent(mComponent->GetType(), mComponent);
	}

	void ComponentRemovedAction::Undo()
	{
		mEntity.AddComponent(mComponent->GetType(), mComponent);
	}
	
	void ComponentRemovedAction::Redo()
	{
		mEntity.RemoveComponent(mComponent->GetType());
	}

	void ComponentModifedAction::Undo()
	{
		IComponent* currComponent = mEntity.GetComponent(mComponent->GetType());
		IComponent* prevComponent = mComponent;

		currComponent = prevComponent;
		mComponent    = currComponent;
	}

	void ComponentModifedAction::Redo()
	{
		// Same effect
		Undo();
	}


}
