#include "cspch.hpp"
#include "EntryPoint.hpp"
#include "App/App.hpp"

using namespace Cosmic;

int32 main(int32 argc, char* argv[])
{
    CS_PROFILE_FN();

    system("dir");
    Application* app = CreateApplication({ argc, argv });
}
