#include "cspch.hpp"
#include "ActionManager.hpp"
#include "ECS/Components.hpp"

#include "Editor/Action/EntityActions.hpp"
#include "Editor/Action/ComponentActions.hpp"

namespace Cosmic
{

	void ActionManager::OnEditorEvent(const IEvent& e)
	{
		EventDispatcher dispatcher(e);
		CS_DISPATCH_EVENT(EntityAddedEvent, OnEntityAdded);
		CS_DISPATCH_EVENT(EntityRemovedEvent, OnEntityRemoved);
		CS_DISPATCH_EVENT(ComponentAddedEvent, OnComponentAdded);
		CS_DISPATCH_EVENT(ComponentRemovedEvent, OnComponentRemoved);
		CS_DISPATCH_EVENT(ComponentModifiedEvent, OnComponentModified);
	}

	void ActionManager::UndoLastAction()
	{
		if (!mLastActionUndone)
		{
			mLastAction->Undo();
			mLastActionUndone = true;
		}

		if (mLastAction->GetPrevAction())
		{
			mLastAction = mLastAction->GetPrevAction();

			mLastActionUndone = false;
			mLastActionRedone = false;
		}
	}

	void ActionManager::RedoLastAction()
	{
		if (!mLastActionRedone)
		{
			mLastAction->Redo();
			mLastActionRedone = true;
		}

		if (mLastAction->GetNextAction())
		{
			mLastAction = mLastAction->GetNextAction();

			mLastActionUndone = false;
			mLastActionRedone = false;
		}
	}


	bool ActionManager::OnEntityAdded(const EntityAddedEvent& e)
	{
		NewAction<EntityAddedAction>(e.GetEntities(), e.GetScene());
		return false;
	}


	bool ActionManager::OnEntityRemoved(const EntityRemovedEvent& e)
	{
		NewAction<EntityRemovedAction>(e.GetEntities(), e.GetScene());
		return false;
	}


	bool ActionManager::OnComponentAdded(const ComponentAddedEvent& e)
	{
		NewAction<ComponentAddedAction>(e.GetEntity(), e.GetComponentPtr());
		return false;
	}


	bool ActionManager::OnComponentRemoved(const ComponentRemovedEvent& e)
	{
		NewAction<ComponentRemovedAction>(e.GetEntity(), e.GetComponentPtr());
		return false;
	}


	bool ActionManager::OnComponentModified(const ComponentModifiedEvent& e)
	{
		NewAction<ComponentModifedAction>(e.GetEntity(), e.GetComponentPtr());
		return false;
	}

}
