#pragma once
#include "Event/Events.hpp"

namespace Cosmic
{

    class Gui
    {
    public:
        static void Init();
        static void Shutdown();
        static void OnEvent(IEvent* e);

    public:
        static void BlockEvents(bool block) { sBlockEvents = block; }

    public:
        static void Begin();
        static void End();

    private:
        inline static bool sBlockEvents = true;
    };

}