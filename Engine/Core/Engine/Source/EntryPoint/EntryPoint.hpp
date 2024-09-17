#pragma once
#include "Base/Types.hpp"

namespace Cosmic
{

    class Application;
    extern Application* CreateApplication();

}

Cosmic::int32 main(Cosmic::int32 argc, char* argv[]);