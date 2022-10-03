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

        TimeUnit currentTime = (float32)glfwGetTime(); // temporary
        
        sCurrentDeltaTime = currentTime - sLastFrameTime;
        sLastFrameTime    = currentTime;

        sFramesPerSecond = 1 / sCurrentDeltaTime;
    }

}