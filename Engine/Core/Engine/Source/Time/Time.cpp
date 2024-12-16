#include "cspch.hpp"
#include "Time/Time.hpp"
#include "App/OS.hpp"
#include <GLFW/glfw3.h>

namespace Cosmic
{

    PersistentRef<Time> Time::Init()
    {
        sInstance = CreatePersistentRef<Time>();

        return sInstance;
    }

    void Time::Shutdown()
    {
        sInstance.Release();
    }
    
    TimeUnit Time::GetCurrentTime()
    {
        return TimeUnit((float32)glfwGetTime()); 
    }

    void Time::Update()
    {
        CS_PROFILE_FN();

        static TimeUnit cumulativeFPS;
        static int32 count = 0;

        if (count != 0)
            sInstance->mAverageFramesPerSecond = (cumulativeFPS + sInstance->mFramesPerSecond) / count;

        count++;


        TimeUnit currentTime = (float32)glfwGetTime(); // temporary
        
        sInstance->mCurrentDeltaTime = currentTime - sInstance->mLastFrameTime;
        sInstance->mLastFrameTime    = currentTime;

        sInstance->mFramesPerSecond = 1 / sInstance->mCurrentDeltaTime;
        cumulativeFPS += sInstance->mFramesPerSecond;
    }

}
