module;
#include "cspch.hpp"
#include "WindowsUtils.hpp"

#include <Windows.h>
#include <windows.h>
#include <profileapi.h>
export module Cosmic.Impl.OS.Windows.WindowsOS;

CS_MODULE_LOG_INFO(Cosmic, Impl.OS.Windows.WindowsOS);

import Cosmic.App.OS;
import Cosmic.App.Log;
import Cosmic.App.ConsoleColor;
import Cosmic.Base;

namespace Cosmic
{

    namespace Utils
    {

        static int32         sCurrentConsoleLinePos = 0;
        static LARGE_INTEGER sStartTime;

        WORD EConsoleColorToWindowsConsoleColor(ConsoleColorPair color)
        {
            CS_PROFILE_FN();

            using enum ::Cosmic::EConsoleColor;

            switch (color.bg)
            {
                case Black:
                {
                switch (color.fg)
                {
                case Black: return 0;
                case BrightBlue: return 1;
                case BrightGreen: return 2;
                case BrightCyan: return 3;
                case BrightRed: return 4;
                case BrightMagenta: return 5;
                case BrightYellow: return 6;
                case BrightWhite: return 7;
                case Gray: return 8;
                case Blue: return 9;
                case Green: return 10;
                case Cyan: return 11;
                case Red: return 12;
                case Magenta: return 13;
                case Yellow: return 14;
                case White: return 15;
                }
            }
                case BrightBlue:
                {
                switch (color.fg)
                {
                case Black:         return 16;
                case BrightBlue:    return 17;
                case BrightGreen:   return 18;
                case BrightCyan:    return 19;
                case BrightRed:     return 20;
                case BrightMagenta: return 21;
                case BrightYellow:  return 22;
                case BrightWhite:   return 23;
                case Gray:          return 24;
                case Blue:          return 25;
                case Green:         return 26;
                case Cyan:          return 27;
                case Red:           return 28;
                case Magenta:       return 29;
                case Yellow:        return 30;
                case White:         return 31;
                }
            }
                case BrightGreen:
                {
                switch (color.fg)
                {
                case Black:         return 32;
                case BrightBlue:    return 33;
                case BrightGreen:   return 34;
                case BrightCyan:    return 35;
                case BrightRed:     return 36;
                case BrightMagenta: return 37;
                case BrightYellow:  return 38;
                case BrightWhite:   return 39;
                case Gray:          return 40;
                case Blue:          return 41;
                case Green:         return 42;
                case Cyan:          return 43;
                case Red:           return 44;
                case Magenta:       return 45;
                case Yellow:        return 46;
                case White:         return 47;
                }
            }
                case BrightCyan:
                {
                switch (color.fg)
                {
                case Black:         return 48;
                case BrightBlue:    return 49;
                case BrightGreen:   return 50;
                case BrightCyan:    return 51;
                case BrightRed:     return 52;
                case BrightMagenta: return 53;
                case BrightYellow:  return 54;
                case BrightWhite:   return 55;
                case Gray:          return 56;
                case Blue:          return 57;
                case Green:         return 58;
                case Cyan:          return 59;
                case Red:           return 60;
                case Magenta:       return 61;
                case Yellow:        return 62;
                case White:         return 63;
                }
            }
                case BrightRed:
                {
                switch (color.fg)
                {
                case Black:         return 64;
                case BrightBlue:    return 65;
                case BrightGreen:   return 66;
                case BrightCyan:    return 67;
                case BrightRed:     return 68;
                case BrightMagenta: return 69;
                case BrightYellow:  return 70;
                case BrightWhite:   return 71;
                case Gray:          return 72;
                case Blue:          return 73;
                case Green:         return 74;
                case Cyan:          return 75;
                case Red:           return 76;
                case Magenta:       return 77;
                case Yellow:        return 78;
                case White:         return 79;
                }
            }
                case BrightMagenta:
                {
                switch (color.fg)
                {
                case Black:         return 80;
                case BrightBlue:    return 81;
                case BrightGreen:   return 82;
                case BrightCyan:    return 83;
                case BrightRed:     return 84;
                case BrightMagenta: return 85;
                case BrightYellow:  return 86;
                case BrightWhite:   return 87;
                case Gray:          return 88;
                case Blue:          return 89;
                case Green:         return 90;
                case Cyan:          return 91;
                case Red:           return 92;
                case Magenta:       return 93;
                case Yellow:        return 94;
                case White:         return 95;
                }
            }
                case BrightYellow:
                {
                switch (color.fg)
                {
                case Black:         return 96;
                case BrightBlue:    return 97;
                case BrightGreen:   return 98;
                case BrightCyan:    return 99;
                case BrightRed:     return 100;
                case BrightMagenta: return 101;
                case BrightYellow:  return 102;
                case BrightWhite:   return 103;
                case Gray:          return 104;
                case Blue:          return 105;
                case Green:         return 106;
                case Cyan:          return 107;
                case Red:           return 108;
                case Magenta:       return 109;
                case Yellow:        return 110;
                case White:         return 111;
                }
            }
                case BrightWhite:
                {
                switch (color.fg)
                {
                case Black:         return 112;
                case BrightBlue:    return 113;
                case BrightGreen:   return 114;
                case BrightCyan:    return 115;
                case BrightRed:     return 116;
                case BrightMagenta: return 117;
                case BrightYellow:  return 118;
                case BrightWhite:   return 119;
                case Gray:          return 120;
                case Blue:          return 121;
                case Green:         return 122;
                case Cyan:          return 123;
                case Red:           return 124;
                case Magenta:       return 125;
                case Yellow:        return 126;
                case White:         return 127;
                }
            }
                case Gray:
                {
                switch (color.fg)
                {
                case Black:         return 128;
                case BrightBlue:    return 129;
                case BrightGreen:   return 130;
                case BrightCyan:    return 131;
                case BrightRed:     return 132;
                case BrightMagenta: return 133;
                case BrightYellow:  return 134;
                case BrightWhite:   return 135;
                case Gray:          return 136;
                case Blue:          return 137;
                case Green:         return 138;
                case Cyan:          return 139;
                case Red:           return 140;
                case Magenta:       return 141;
                case Yellow:        return 142;
                case White:         return 143;
                }
            }
                case Blue:
                {
                switch (color.fg)
                {
                case Black:         return 144;
                case BrightBlue:    return 145;
                case BrightGreen:   return 146;
                case BrightCyan:    return 147;
                case BrightRed:     return 148;
                case BrightMagenta: return 149;
                case BrightYellow:  return 150;
                case BrightWhite:   return 151;
                case Gray:          return 152;
                case Blue:          return 153;
                case Green:         return 154;
                case Cyan:          return 155;
                case Red:           return 156;
                case Magenta:       return 157;
                case Yellow:        return 158;
                case White:         return 159;
                }
            }
                case Green:
                {
                switch (color.fg)
                {
                case Black:         return 160;
                case BrightBlue:    return 161;
                case BrightGreen:   return 162;
                case BrightCyan:    return 163;
                case BrightRed:     return 164;
                case BrightMagenta: return 165;
                case BrightYellow:  return 166;
                case BrightWhite:   return 167;
                case Gray:          return 168;
                case Blue:          return 169;
                case Green:         return 170;
                case Cyan:          return 171;
                case Red:           return 172;
                case Magenta:       return 173;
                case Yellow:        return 174;
                case White:         return 175;
                }
            }
                case Cyan:
                {
                switch (color.fg)
                {
                case Black:         return 176;
                case BrightBlue:    return 177;
                case BrightGreen:   return 178;
                case BrightCyan:    return 179;
                case BrightRed:     return 180;
                case BrightMagenta: return 181;
                case BrightYellow:  return 182;
                case BrightWhite:   return 183;
                case Gray:          return 184;
                case Blue:          return 185;
                case Green:         return 186;
                case Cyan:          return 187;
                case Red:           return 188;
                case Magenta:       return 189;
                case Yellow:        return 190;
                case White:         return 191;
                }
            }
                case Red:
                {
                switch (color.fg)
                {
                case Black:         return 192;
                case BrightBlue:    return 193;
                case BrightGreen:   return 194;
                case BrightCyan:    return 195;
                case BrightRed:     return 196;
                case BrightMagenta: return 197;
                case BrightYellow:  return 198;
                case BrightWhite:   return 199;
                case Gray:          return 200;
                case Blue:          return 201;
                case Green:         return 202;
                case Cyan:          return 203;
                case Red:           return 204;
                case Magenta:       return 205;
                case Yellow:        return 206;
                case White:         return 207;
                }
            }
                case Magenta:
                {
                switch (color.fg)
                {
                case Black:         return 208;
                case BrightBlue:    return 209;
                case BrightGreen:   return 210;
                case BrightCyan:    return 211;
                case BrightRed:     return 212;
                case BrightMagenta: return 213;
                case BrightYellow:  return 214;
                case BrightWhite:   return 215;
                case Gray:          return 216;
                case Blue:          return 217;
                case Green:         return 218;
                case Cyan:          return 219;
                case Red:           return 220;
                case Magenta:       return 221;
                case Yellow:        return 222;
                case White:         return 223;
                }
            }
                case Yellow:
                {
                switch (color.fg)
                {
                case Black:         return 224;
                case BrightBlue:    return 225;
                case BrightGreen:   return 226;
                case BrightCyan:    return 227;
                case BrightRed:     return 228;
                case BrightMagenta: return 229;
                case BrightYellow:  return 230;
                case BrightWhite:   return 231;
                case Gray:          return 232;
                case Blue:          return 233;
                case Green:         return 234;
                case Cyan:          return 235;
                case Red:           return 236;
                case Magenta:       return 237;
                case Yellow:        return 238;
                case White:         return 239;
                }
            }
                case White:
                {
                switch (color.fg)
                {
                case Black:         return 240;
                case BrightBlue:    return 241;
                case BrightGreen:   return 242;
                case BrightCyan:    return 243;
                case BrightRed:     return 244;
                case BrightMagenta: return 245;
                case BrightYellow:  return 246;
                case BrightWhite:   return 247;
                case Gray:          return 248;
                case Blue:          return 249;
                case Green:         return 250;
                case Cyan:          return 251;
                case Red:           return 252;
                case Magenta:       return 253;
                case Yellow:        return 254;
                case White:         return 255;
                }
            }
            }

        }
    }

    void OS::Init()
    {
        CS_PROFILE_FN();

        // Get the start time to get the current time later on.
        QueryPerformanceCounter(&Utils::sStartTime);
    }

    void OS::Shutdown()
    {
        CS_PROFILE_FN();
    }

    void OS::FlushConsole()
    {
        CS_PROFILE_FN();

        Utils::sCurrentConsoleLinePos = 0;

        COORD                      topLeft  = { 0, 0 };
        HANDLE                     console  = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO screen;
        DWORD                      written;

        CS_WINDOWS_CALL(GetConsoleScreenBufferInfo(console, &screen),                                                                                                   "Unable to obtain console information.");
        CS_WINDOWS_CALL(FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written),                                                "Unable to clear the console.");
        CS_WINDOWS_CALL(FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written), "Unable to clear the console colors.");
        CS_WINDOWS_CALL(SetConsoleCursorPosition(console, topLeft),                                                                                                     "Unable to reset the console cursor position.");

        SetConsoleColor({ EConsoleColor::White, EConsoleColor::Black });
    }

    void OS::FlushConsoleLine()
    {
        CS_PROFILE_FN();

        COORD                      coord   = { 0, (SHORT)Utils::sCurrentConsoleLinePos };
        HANDLE                     console = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO screen;
        DWORD                      written;

        CS_WINDOWS_CALL(GetConsoleScreenBufferInfo (console, &screen),                                                                                                 "Unable to obtain console information.");
        CS_WINDOWS_CALL(FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, coord, &written),                                                 "Unable to clear the console line.");
        CS_WINDOWS_CALL(FillConsoleOutputAttribute (console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, coord, &written), "Unable to clear the console line colors.");
        CS_WINDOWS_CALL(SetConsoleCursorPosition   (console, coord),                                                                                                   "Unable to set the console cursor position.");

        SetConsoleColor({ EConsoleColor::White, EConsoleColor::Black });
    }

    void OS::SetConsoleColor(ConsoleColorPair color)
    {
        CS_PROFILE_FN();

        WORD wAttributes = Utils::EConsoleColorToWindowsConsoleColor(color);
        CS_WINDOWS_CALL(SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wAttributes), "Unable to set the console color.");
    }

    void OS::Print(const char* text)
    {
        CS_PROFILE_FN();

        // TODO: Check if `text` contains '\n'
        Utils::sCurrentConsoleLinePos++;

        CS_WINDOWS_CALL(WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), text, (DWORD)strlen(text), nullptr, nullptr), "Unable to log to the console.");
    }

#undef GetCurrentTime
    float32 OS::GetCurrentTime()
    {
        CS_PROFILE_FN();

        LARGE_INTEGER currentTime, frequency;
        QueryPerformanceCounter(&currentTime);
        QueryPerformanceFrequency(&frequency);

        return static_cast<float32>((currentTime.QuadPart - Utils::sStartTime.QuadPart) / (double)frequency.QuadPart);
    }

}