#pragma once
#include "Base/Base.hpp"

namespace Cosmic
{

	enum class EEditorActionType : int16
	{
		None = -1,
		EntityAdded, EntityRemoved
	};

	class IEditorAction
	{
	public:
		IEditorAction()          = default;
		virtual ~IEditorAction() = default;

		void SetPrevAction(IEditorAction* prev)
		{
			mPrevAction = prev;

			if (prev)
				prev->mNextAction = this;
		}

	public:
		virtual void Undo() { }
		virtual void Redo() { }

		virtual EEditorActionType GetType() { return EEditorActionType::None; }

	public:
		IEditorAction* GetNextAction() { return mNextAction; }
		IEditorAction* GetPrevAction() { return mPrevAction; }

		template<typename T>
		T* As()
		{
			if (GetType() == T::GetStaticType())
				return static_cast<T*>(this);

			return nullptr;
		}

	private:
		IEditorAction* mNextAction = nullptr;
		IEditorAction* mPrevAction = nullptr;
	};

}
