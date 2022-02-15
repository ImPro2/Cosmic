module;
#include "cspch.hpp"
#include <functional>
export module Cosmic.App.Event;

import Cosmic.Base;

namespace Cosmic
{
#if 0
    template<typename T> struct Event;

    using EventListener = std::function<void(Event&)>;

    template<typename T>
    struct Event
    {
    private:
        std::queue<
        std::queue<EventListener> mListeners;
    };
#endif
}