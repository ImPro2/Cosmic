#pragma once
#include "App/Event/Events.hpp"
#include "Editor/Event/EditorEvents.hpp"
#include "Editor/Event/EditorSceneEvents.hpp"

#include "IEditorAction.hpp"
#include "EntityActions.hpp"

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
		bool OnEntityAdded(const EditorEntityAddedEvent& e);
		bool OnEntityRemoved(const EditorEntityRemovedEvent& e);

	private:
		IEditorAction* mFirstAction = nullptr;
		IEditorAction* mLastAction  = nullptr;
	};

}
