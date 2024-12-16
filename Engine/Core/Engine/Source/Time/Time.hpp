#pragma once
#include "Base/Base.hpp"
#include "Time/DeltaTime.hpp"
#include "Memory/SmartPtrs.hpp"

namespace Cosmic
{

#undef GetCurrentTime

    class Application;

    class Time : public IRefCounted
    {
    public:
        static TimeUnit  GetCurrentTime();
        static DeltaTime GetDeltaTime()     { return sInstance->mCurrentDeltaTime;       }
        static TimeUnit  GetLastFrameTime() { return sInstance->mLastFrameTime;          }
        static TimeUnit  GetTime()          { return TimeUnit(OS::GetCurrentTime());     }
        static TimeUnit  GetFPS()           { return sInstance->mFramesPerSecond;        }
        static TimeUnit  GetAverageFPS()    { return sInstance->mAverageFramesPerSecond; }

    private:
        static Ref<Time> Init();
        static void      Shutdown();

        static void Update();

    private:
        TimeUnit  mLastFrameTime;      // time last frame started
        DeltaTime mCurrentDeltaTime;
        TimeUnit  mFramesPerSecond;
        TimeUnit  mAverageFramesPerSecond;

        inline static Ref<Time> sInstance;

        friend class Application;
    };

}
