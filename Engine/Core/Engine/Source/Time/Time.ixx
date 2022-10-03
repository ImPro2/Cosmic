module;
#include "cspch.hpp"
export module Cosmic.Time;

import Cosmic.Time.DeltaTime;
import Cosmic.App.OS;
import Cosmic.Base;

export import Cosmic.Time.TimeUnit;
export import Cosmic.Time.DeltaTime;
export import Cosmic.Time.Timer;

namespace Cosmic
{

#undef GetCurrentTime

    class Application;

    export class Time
    {
    public:
        static DeltaTime GetDeltaTime()     { return sCurrentDeltaTime;              }
        static TimeUnit  GetLastFrameTime() { return sLastFrameTime;                 }
        static TimeUnit  GetTime()          { return TimeUnit(OS::GetCurrentTime()); }

    private:
        static void Update();

    private:
        inline static TimeUnit  sLastFrameTime;      // time it took for the last frame to complete
        inline static DeltaTime sCurrentDeltaTime;

    private:
        friend class Application;
    };

}