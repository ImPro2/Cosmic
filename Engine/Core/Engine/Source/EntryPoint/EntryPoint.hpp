#pragma once
#include "Base/Types.hpp"

namespace Cosmic
{

    class Application;

    struct StartupArguments
    {
        int32 ArgumentCount;
        char** Arguments;
    };

    extern Application* CreateApplication(StartupArguments&& args);

}

Cosmic::int32 main(Cosmic::int32 argc, char* argv[]);

