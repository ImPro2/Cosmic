#pragma once
#include "App/Log/ConsoleColor.hpp"
#include "Base/Singleton.hpp"
#include "Base/Base.hpp"

namespace Cosmic
{
    class OS
    {
    public:
        static void Init();
        static void Shutdown();

        static void FlushConsole();
        static void FlushConsoleLine();
        static void SetConsoleColor(ConsoleColorPair color);
        static void Print(const char* text);

        static float32 GetCurrentTime();

        static String OpenFileDialog(const char* filter);
        static String SaveFileDialog(const char* filter);

        static void* LoadDynamicLibrary(const char* path);
        static void  FreeDynamicLibrary(void* library);
        static void* RetrieveFunctionFromDynamicLibrary(const char* name, void* library);
    };
}