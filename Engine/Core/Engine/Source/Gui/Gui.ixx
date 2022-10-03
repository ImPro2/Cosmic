module;
#include "cspch.hpp"
export module Cosmic.Gui;

namespace Cosmic
{

    class Event;

    export class Gui
    {
    public:
        static void Init();
        static void Shutdown();
        static void OnEvent(const Event& e);

    public:
        static void BlockEvents(bool block) { sBlockEvents = block; }

    public:
        static void Begin();
        static void End();

    private:
        inline static bool sBlockEvents = true;
    };

}