#pragma once
#include "Event/Events.hpp"
#include "Memory/SmartPtrs.hpp"

namespace Cosmic
{

    class Application;

    class Gui : public IRefCounted
    {
    public:
        static void BlockEvents(bool block) { sInstance->mBlockEvents = block; }

    private:
        static Ref<Gui> Init();
        static void     Shutdown();

        static void OnEvent(IEvent* e);

    private:
        static void Begin();
        static void End();

    private:
        bool mInitialized = false;
        bool mBlockEvents = true;

        inline static Ref<Gui> sInstance;

        friend class Application;
    };

}