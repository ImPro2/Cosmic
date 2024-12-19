#pragma once
#include "EntryPoint/StartupArgumentList.hpp"

namespace Cosmic
{

    class Application;

    extern Application* CreateApplication(StartupArgumentList&& args);

}

Cosmic::int32 main(Cosmic::int32 argc, char* argv[]);

