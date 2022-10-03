module;
#include "cspch.hpp"
export module Cosmic.Time.DeltaTime;

import Cosmic.Time.TimeUnit;
import Cosmic.Base;

namespace Cosmic
{

    export class DeltaTime
    {
    public:
        DeltaTime(float32 dtInSeconds  = 0.0f) : mDeltaTime(dtInSeconds) { }

    public:
        operator float32() const { return mDeltaTime; }

    private:
        TimeUnit mDeltaTime;
    };

    export using Dt = DeltaTime;

}