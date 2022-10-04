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
        static TimeUnit  GetFPS()           { return sFramesPerSecond;               }
        static TimeUnit  GetAverageFPS()    { return sAverageFramesPerSecond;        }

    private:
        static void Update();

    private:
        inline static TimeUnit  sLastFrameTime;      // time last frame started
        inline static DeltaTime sCurrentDeltaTime;
        inline static TimeUnit  sFramesPerSecond;
        inline static TimeUnit  sAverageFramesPerSecond;

    private:
        friend class Application;
    };

}