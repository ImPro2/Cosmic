#pragma once

namespace Cosmic
{

    enum class EConsoleColor
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

    struct ConsoleColorPair
    {
        EConsoleColor fg, bg;
    };

}