#pragma once
#include "App/Event/Events.hpp"
#include "Editor/Event/EditorEvents.hpp"
#include "Editor/Event/SceneEvents.hpp"
#include "Editor/Event/EntityEvents.hpp"
#include "Editor/Event/ComponentEvents.hpp"

#include "IEditorAction.hpp"

namespace Cosmic
{

	class ActionManager
	{
	public:
		void OnEditorEvent(const IEvent& e);

		void UndoLastAction();
		void RedoLastAction();

	private:
		template<typename T, typename ... Args>
		void NewAction(Args&& ... args)
		{
			mLastActionUndone = false;
			mLastActionRedone = false;

			// Remove all next actions of mLastAction
			if (mLastAction)
				DeleteNextActionsRecurse(mLastAction->GetNextAction());

			T* action = new T(std::forward<Args>(args)...);
			action->SetPrevAction(mLastAction);
			mLastAction = action;
		}

		void DeleteNextActionsRecurse(IEditorAction* curr)
		{
			if (!curr) return;

			IEditorAction* next = curr->GetNextAction();

			if (next)
				DeleteNextActionsRecurse(next);

			delete curr;
		}

	private:
		bool OnEntityAdded(const EntityAddedEvent& e);
		bool OnEntityRemoved(const EntityRemovedEvent& e);
		bool OnComponentAdded(const ComponentAddedEvent& e);
		bool OnComponentRemoved(const ComponentRemovedEvent& e);
		bool OnComponentModified(const ComponentModifiedEvent& e);

	private:
		IEditorAction* mLastAction       = nullptr;
		bool           mLastActionUndone = false;
		bool           mLastActionRedone = false;
	};

}
