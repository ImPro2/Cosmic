module;
#include "cspch.hpp"
#include <functional>
export module Cosmic.App.Events;

export import Cosmic.App.WindowEvents;

import Cosmic.App.WindowEvents;

namespace Cosmic
{

	export class EventDispatcher
	{
	public:
		EventDispatcher(const WindowEvent& event)
			: m_Event(const_cast<WindowEvent&>(event))
		{
		}

		// F will be deduced by the compiler
		template<typename T, typename F>
		void Dispatch(const F& func)
		{
			if (m_Event.GetType() == T::GetStaticType())
			{
				func(static_cast<T&>(m_Event));
			}
		}
	private:
		WindowEvent& m_Event;
	};

}