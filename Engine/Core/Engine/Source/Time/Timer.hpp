#pragma once

#include "Base/Base.hpp"
#include "Time/TimeUnit.hpp"
#include "App/OS.hpp"
#include "App/Log/Log.hpp"

namespace Cosmic
{

    // Logs the time duration to the console when it is out of scope
    class ScopedTimer
    {
    public:
        ScopedTimer(const String& moduleName, String&& name)
            : mStart(OS::GetCurrentTime()),
              mModuleName(name),
              mName(name)
        {
        }

        ~ScopedTimer()
        {
            TimeUnit now     = OS::GetCurrentTime();
            TimeUnit elapsed = now - mStart;

            Log::Info("[{}]: Timer \"{}\" elapsed {} seconds.", mModuleName.c_str(), mName.c_str(), (float32)elapsed);
        }

    private:
        TimeUnit mStart;
        String   mModuleName = "";
        String   mName       = "";
    };

}