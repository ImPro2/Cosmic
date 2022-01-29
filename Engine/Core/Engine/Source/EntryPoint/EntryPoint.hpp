#pragma once

import Cosmic.Base.Types;

namespace Cosmic
{

    class Application;
    extern Application* CreateApplication();

}

Cosmic::int32 main(Cosmic::int32 argc, char* argv[]);