module;
#include "cspch.hpp"
#include <queue>
#include <functional>
#include <vector>
export module Cosmic.App.EventSystem;

namespace Cosmic
{

    export enum class EEventType
    {
        Struct0, Struct1
    };

    export struct Struct0
    {
        int i = 0;

        static EEventType GetType() { return EEventType::Struct0; }
    };

    export struct Struct1
    {
        const char* c = "";

        static EEventType GetType() { return EEventType::Struct1; }
    };

    export struct EventBus
    {
        std::queue<Struct0> Struct0Events;
        std::queue<Struct1> Struct1Events;

        std::vector<std::function<void(Struct0&)>> Struct0Functions;
        std::vector<std::function<void(Struct1&)>> Struct1Functions;
    };

    export class EventSystem
    {
    public:
        template<typename T>
        static void Submit(T e)
        {
            static_assert(false);
        }

        template<>
        static void Submit<Struct0>(Struct0 e)
        {
            sEventBus.Struct0Events.push(e);
        }

        template<>
        static void Submit<Struct1>(Struct1 e)
        {
            sEventBus.Struct1Events.push(e);
        }

        template<typename T>
        static void On(std::function<void(T&)> fn)
        {
            static_assert(false);
        }

        template<>
        static void On<Struct0>(std::function<void(Struct0&)> fn)
        {
            sEventBus.Struct0Functions.push_back(fn);
        }

        template<>
        static void On<Struct1>(std::function<void(Struct1&)> fn)
        {
            sEventBus.Struct1Functions.push_back(fn);
        }

        static void Update()
        {
            while (!sEventBus.Struct0Events.empty())
            {
                for (auto fn : sEventBus.Struct0Functions)
                    fn(sEventBus.Struct0Events.front());

                sEventBus.Struct0Events.pop();
            }

            sEventBus.Struct0Functions.clear();

            while (!sEventBus.Struct1Events.empty())
            {
                for (auto fn : sEventBus.Struct1Functions)
                    fn(sEventBus.Struct1Events.front());

                sEventBus.Struct1Events.pop();
            }

            sEventBus.Struct1Functions.clear();
        }

    private:
        inline static EventBus sEventBus;
    };

}