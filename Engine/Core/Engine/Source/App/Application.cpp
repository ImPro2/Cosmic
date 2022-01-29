module;
#include "cspch.hpp"
module Cosmic.App.Application;
CS_MODULE_LOG_INFO(Cosmic, App.Application);

import Cosmic.App.Log;

namespace Cosmic
{

    Application::Application()
    {
        CS_ASSERT(!sInstance, "Already initialized the `Application` class.");
        sInstance = this;
    }

    Application::~Application()
    {

    }

    void Application::Init(ApplicationInfo&& info)
    {
        mInfo = std::move(info);

        Run();
    }

    void Application::Close()
    {

    }

    void Application::Run()
    {
        while (true);
    }

}