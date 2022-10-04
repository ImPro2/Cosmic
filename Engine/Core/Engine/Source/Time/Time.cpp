module;
#include "cspch.hpp"
#include <GLFW/glfw3.h>
module Cosmic.Time;

import Cosmic.App.Application;

namespace Cosmic
{

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