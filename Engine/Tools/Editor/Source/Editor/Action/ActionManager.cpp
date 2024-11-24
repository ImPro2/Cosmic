#include "cspch.hpp"
#include "ActionManager.hpp"

namespace Cosmic
{

	void ActionManager::OnEditorEvent(const IEvent& e)
	{
		EventDispatcher dispatcher(e);
		CS_DISPATCH_EVENT(EditorEntityAddedEvent, OnEntityAdded);
		CS_DISPATCH_EVENT(EditorEntityRemovedEvent, OnEntityRemoved);
	}

	void ActionManager::UndoLastAction()
	{
		mLastAction->Undo();
	}


	void ActionManager::RedoLastAction()
	{
		mLastAction->Redo();
	}


	bool ActionManager::OnEntityAdded(const EditorEntityAddedEvent& e)
	{
		NewAction<EntityAddedAction>(e.GetEntities(), e.GetScene());
		return false;
	}


	bool ActionManager::OnEntityRemoved(const EditorEntityRemovedEvent& e)
	{
		NewAction<EntityRemovedAction>(e.GetEntities(), e.GetScene());
		return false;
	}

}
