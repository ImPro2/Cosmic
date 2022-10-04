module;
#include "cspch.hpp"
#include <functional>
export module Cosmic.Time.Timer;

import Cosmic.Time.TimeUnit;
import Cosmic.App.Log;
import Cosmic.App.OS;
import Cosmic.Base;

namespace Cosmic
{

    // Logs the time duration to the console when it is out of scope
    export class ScopedTimer
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