#include "cspch.hpp"
#include "Time/Time.hpp"
#include "App/OS.hpp"
#include <GLFW/glfw3.h>

namespace Cosmic
{
    
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
            sAverageFramesPerSecond = (cumulativeFPS + sFramesPerSecond) / count;

        count++;


        TimeUnit currentTime = (float32)glfwGetTime(); // temporary
        
        sCurrentDeltaTime = currentTime - sLastFrameTime;
        sLastFrameTime    = currentTime;

        sFramesPerSecond = 1 / sCurrentDeltaTime;
        cumulativeFPS += sFramesPerSecond;
    }

}
