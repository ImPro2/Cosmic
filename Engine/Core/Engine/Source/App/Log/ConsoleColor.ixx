module;
#include "cspch.hpp"
export module Cosmic.App.ConsoleColor;

namespace Cosmic
{

    export enum class EConsoleColor
    {
        Black,
        BrightBlue,
        BrightGreen,
        BrightCyan,
        BrightRed,
        BrightMagenta,
        BrightYellow,
        BrightWhite,
        Gray,
        Blue,
        Green,
        Cyan,
        Red,
        Magenta,
        Yellow,
        White
    };

    export struct ConsoleColorPair
    {
        EConsoleColor fg, bg;
    };

}