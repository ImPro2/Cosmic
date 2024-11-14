#include "Base/Macros.hpp"
#include "cspch.hpp"

#ifdef CS_PLATFORM_LINUX

#include "App/OS.hpp"

#include <stdio.h>

CS_MODULE_LOG_INFO(Cosmic, Impl.OS.Linux.LinuxOS);

namespace Cosmic {
    
    namespace Util
    {
        
        static uint8 FgColorToLinuxColor(EConsoleColor color)
        {
            switch (color)
            {
                case EConsoleColor::Black:         return 30;
                case EConsoleColor::BrightBlue:    return 94;
                case EConsoleColor::BrightGreen:   return 92;
                case EConsoleColor::BrightCyan:    return 96;
                case EConsoleColor::BrightRed:     return 91;
                case EConsoleColor::BrightMagenta: return 95;
                case EConsoleColor::BrightYellow:  return 93;
                case EConsoleColor::BrightWhite:   return 0;
                case EConsoleColor::Gray:          return 90;
                case EConsoleColor::Blue:          return 34;
                case EConsoleColor::Green:         return 32;
                case EConsoleColor::Cyan:          return 36;
                case EConsoleColor::Red:           return 31;
                case EConsoleColor::Magenta:       return 35;
                case EConsoleColor::Yellow:        return 33;
                case EConsoleColor::White:         return 37;
            }

            return 0;
        }

        static uint8 BgColorToLinuxColor(EConsoleColor color)
        {
            switch (color)
            {
                case EConsoleColor::Black:         return 40;
                case EConsoleColor::BrightBlue:    return 104;
                case EConsoleColor::BrightGreen:   return 102;
                case EConsoleColor::BrightCyan:    return 106;
                case EConsoleColor::BrightRed:     return 101;
                case EConsoleColor::BrightMagenta: return 105;
                case EConsoleColor::BrightYellow:  return 103;
                case EConsoleColor::BrightWhite:   return 7;
                case EConsoleColor::Gray:          return 100;
                case EConsoleColor::Blue:          return 44;
                case EConsoleColor::Green:         return 42;
                case EConsoleColor::Cyan:          return 46;
                case EConsoleColor::Red:           return 41;
                case EConsoleColor::Magenta:       return 45;
                case EConsoleColor::Yellow:        return 43;
                case EConsoleColor::White:         return 107;
            }

            return 0;
        }

    }

    static ConsoleColorPair sCurrentConsoleColor = { EConsoleColor::White, EConsoleColor::Black };

    void OS::Init() 
    {
    }

    void OS::Shutdown() {}

    void OS::FlushConsole() {}

    void OS::FlushConsoleLine() {}

    void OS::SetConsoleColor(ConsoleColorPair color)
    {
        sCurrentConsoleColor = color;
    }

    void OS::Print(const char *text)
    {
        uint8 fg = Util::FgColorToLinuxColor(sCurrentConsoleColor.fg);
        uint8 bg = Util::BgColorToLinuxColor(sCurrentConsoleColor.bg);

        fprintf(stdout, "\033[%i;%im %s\033[m", fg, bg, text);
    }

    float32 OS::GetCurrentTime() {}

    String OS::OpenFileDialog(const char *filter) {}

    String OS::SaveFileDialog(const char *filter) {}

    void *OS::LoadDynamicLibrary(const char *path) {}

    void OS::FreeDynamicLibrary(void *library) {}

    void* OS::RetrieveFunctionFromDynamicLibrary(const char *name, void *library)
    {
    }

}

#endif
