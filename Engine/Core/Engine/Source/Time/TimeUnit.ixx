module;
#include "cspch.hpp"
export module Cosmic.Time.TimeUnit;

import Cosmic.Base;

namespace Cosmic
{

    export class TimeUnit
    {
    public:
        TimeUnit(float32 timeInSeconds = 0.0f) : mTime(timeInSeconds) {}

    public:
        float32 InHours()        { return InMinutes() / 60.0f;        }
        float32 InMinutes()      { return InSeconds() / 60.0f;        }
        float32 InSeconds()      { return mTime;                      }
        float32 InMilliSeconds() { return InSeconds()      * 1000.0f; }
        float32 InMicroSeconds() { return InMilliSeconds() * 1000.0f; }
        float32 InNanoSeconds()  { return InMicroSeconds() * 1000.0f; }

        operator float32() const { return mTime; }
        void operator+=(TimeUnit other) { mTime += (float32)other; }

    protected:
        float32 mTime = 0.0f;
    };

}