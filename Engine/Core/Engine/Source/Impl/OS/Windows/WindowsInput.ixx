module;
#include "cspch.hpp"
#include "WindowsUtils.hpp"
export module Cosmic.Impl.OS.Windows.WindowsInput;

import Cosmic.App.Input;
import Cosmic.App.KeyAndMouseCodes;
import Cosmic.Base;

namespace Cosmic
{

    namespace Utils
    {
        static int32 EKeyCodeToWindowsKeyCode(EKeyCode code)
        {
            CS_PROFILE_FN();

            switch (code)
            {
                case EKeyCode::Space:          return 32;
                case EKeyCode::Apostrophe:     return -1;
                case EKeyCode::Comma:          return 188;
                case EKeyCode::Minus:          return 189;
                case EKeyCode::Period:         return 190;
                case EKeyCode::Slash:          return -1;
                case EKeyCode::D0:             return 48;
                case EKeyCode::D1:             return 49;
                case EKeyCode::D2:             return 50;
                case EKeyCode::D3:             return 51;
                case EKeyCode::D4:             return 52;
                case EKeyCode::D5:             return 53;
                case EKeyCode::D6:             return 54;
                case EKeyCode::D7:             return 55;
                case EKeyCode::D8:             return 56;
                case EKeyCode::D9:             return 57;
                case EKeyCode::Semicolon:      return 186;
                case EKeyCode::Equal:          return -1;
                case EKeyCode::A:              return 65;
                case EKeyCode::B:              return 66;
                case EKeyCode::C:              return 67;
                case EKeyCode::D:              return 68;
                case EKeyCode::E:              return 69;
                case EKeyCode::F:              return 70;
                case EKeyCode::G:              return 71;
                case EKeyCode::H:              return 72;
                case EKeyCode::I:              return 73;
                case EKeyCode::J:              return 74;
                case EKeyCode::K:              return 75;
                case EKeyCode::L:              return 76;
                case EKeyCode::M:              return 77;
                case EKeyCode::N:              return 78;
                case EKeyCode::O:              return 79;
                case EKeyCode::P:              return 80;
                case EKeyCode::Q:              return 81;
                case EKeyCode::R:              return 82;
                case EKeyCode::S:              return 83;
                case EKeyCode::T:              return 84;
                case EKeyCode::U:              return 85;
                case EKeyCode::V:              return 86;
                case EKeyCode::W:              return 87;
                case EKeyCode::X:              return 88;
                case EKeyCode::Y:              return 89;
                case EKeyCode::Z:              return 90;
                case EKeyCode::LeftBracket:    return 219;
                case EKeyCode::Backslash:      return 220;
                case EKeyCode::RightBracket:   return 221;
                case EKeyCode::GraveAccent:    return 222;
                case EKeyCode::World1:         return -1;
                case EKeyCode::World2:         return -1;
                case EKeyCode::Escape:         return 27;
                case EKeyCode::Enter:          return 13;
                case EKeyCode::Tab:            return 9;
                case EKeyCode::Backspace:      return 8;
                case EKeyCode::Insert:         return 45;
                case EKeyCode::Delete:         return 46;
                case EKeyCode::Right:          return 39;
                case EKeyCode::Left:           return 37;
                case EKeyCode::Down:           return 40;
                case EKeyCode::Up:             return 38;
                case EKeyCode::PageUp:         return 33;
                case EKeyCode::PageDown:       return 34;
                case EKeyCode::Home:           return 36;
                case EKeyCode::End:            return 35;
                case EKeyCode::CapsLock:       return 20;
                case EKeyCode::ScrollLock:     return 145;
                case EKeyCode::NumLock:        return 144;
                case EKeyCode::PrintScreen:    return 44;
                case EKeyCode::Pause:          return 19;
                case EKeyCode::F1:             return 112;
                case EKeyCode::F2:             return 113;
                case EKeyCode::F3:             return 114;
                case EKeyCode::F4:             return 115;
                case EKeyCode::F5:             return 116;
                case EKeyCode::F6:             return 117;
                case EKeyCode::F7:             return 118;
                case EKeyCode::F8:             return 119;
                case EKeyCode::F9:             return 120;
                case EKeyCode::F10:            return 121;
                case EKeyCode::F11:            return 122;
                case EKeyCode::F12:            return 123;
                case EKeyCode::F13:            return 124;
                case EKeyCode::F14:            return 125;
                case EKeyCode::F15:            return 126;
                case EKeyCode::F16:            return 127;
                case EKeyCode::F17:            return 128;
                case EKeyCode::F18:            return 129;
                case EKeyCode::F19:            return 130;
                case EKeyCode::F20:            return 131;
                case EKeyCode::F21:            return 132;
                case EKeyCode::F22:            return 133;
                case EKeyCode::F23:            return 134;
                case EKeyCode::F24:            return 135;
                case EKeyCode::F25:            return 136;
                case EKeyCode::KP0:            return 96;
                case EKeyCode::KP1:            return 97;
                case EKeyCode::KP2:            return 98;
                case EKeyCode::KP3:            return 99;
                case EKeyCode::KP4:            return 100;
                case EKeyCode::KP5:            return 101;
                case EKeyCode::KP6:            return 102;
                case EKeyCode::KP7:            return 103;
                case EKeyCode::KP8:            return 104;
                case EKeyCode::KP9:            return 105;
                case EKeyCode::KPDecimal:      return 110;
                case EKeyCode::KPDivide:       return 111;
                case EKeyCode::KPMultiply:     return 106;
                case EKeyCode::KPSubtract:     return 109;
                case EKeyCode::KPAdd:          return 107;
                case EKeyCode::KPEnter:        return -1;
                case EKeyCode::KPEqual:        return 108;
                case EKeyCode::LeftShift:      return 160;
                case EKeyCode::LeftControl:    return 162;
                case EKeyCode::LeftAlt:        return 164;
                case EKeyCode::LeftSuper:      return -1;
                case EKeyCode::RightShift:     return 161;
                case EKeyCode::RightControl:   return 163;
                case EKeyCode::RightAlt:       return 165;
                case EKeyCode::RightSuper:     return -1;
                case EKeyCode::Menu:           return -1;
            }
        }

        static EKeyCode WindowsKeyToEKeyCode(int32 key)
        {
            CS_PROFILE_FN();

            switch (key)
            {
                case 32:    return EKeyCode::Space;
                case 188:   return EKeyCode::Comma;
                case 189:   return EKeyCode::Minus;
                case 190:   return EKeyCode::Period;
                case 48:    return EKeyCode::D0;
                case 49:    return EKeyCode::D1;
                case 50:    return EKeyCode::D2;
                case 51:    return EKeyCode::D3;
                case 52:    return EKeyCode::D4;
                case 53:    return EKeyCode::D5;
                case 54:    return EKeyCode::D6;
                case 55:    return EKeyCode::D7;
                case 56:    return EKeyCode::D8;
                case 57:    return EKeyCode::D9;
                case 186:   return EKeyCode::Semicolon;
                case 65:    return EKeyCode::A;
                case 66:    return EKeyCode::B;
                case 67:    return EKeyCode::C;
                case 68:    return EKeyCode::D;
                case 69:    return EKeyCode::E;
                case 70:    return EKeyCode::F;
                case 71:    return EKeyCode::G;
                case 72:    return EKeyCode::H;
                case 73:    return EKeyCode::I;
                case 74:    return EKeyCode::J;
                case 75:    return EKeyCode::K;
                case 76:    return EKeyCode::L;
                case 77:    return EKeyCode::M;
                case 78:    return EKeyCode::N;
                case 79:    return EKeyCode::O;
                case 80:    return EKeyCode::P;
                case 81:    return EKeyCode::Q;
                case 82:    return EKeyCode::R;
                case 83:    return EKeyCode::S;
                case 84:    return EKeyCode::T;
                case 85:    return EKeyCode::U;
                case 86:    return EKeyCode::V;
                case 87:    return EKeyCode::W;
                case 88:    return EKeyCode::X;
                case 89:    return EKeyCode::Y;
                case 90:    return EKeyCode::Z;
                case 219:   return EKeyCode::LeftBracket;
                case 220:   return EKeyCode::Backslash;
                case 221:   return EKeyCode::RightBracket;
                case 222:   return EKeyCode::GraveAccent;
                case 27:    return EKeyCode::Escape;
                case 13:    return EKeyCode::Enter;
                case 9:     return EKeyCode::Tab;
                case 8:     return EKeyCode::Backspace;
                case 45:    return EKeyCode::Insert;
                case 46:    return EKeyCode::Delete;
                case 39:    return EKeyCode::Right;
                case 37:    return EKeyCode::Left;
                case 40:    return EKeyCode::Down;
                case 38:    return EKeyCode::Up;
                case 33:    return EKeyCode::PageUp;
                case 34:    return EKeyCode::PageDown;
                case 36:    return EKeyCode::PageDown;
                case 35:    return EKeyCode::Home;
                case 20:    return EKeyCode::End;
                case 145:   return EKeyCode::CapsLock;
                case 144:   return EKeyCode::ScrollLock;
                case 44:    return EKeyCode::NumLock;
                case 19:    return EKeyCode::PrintScreen;
                case 112:   return EKeyCode::Pause;
                case 113:   return EKeyCode::F1;
                case 114:   return EKeyCode::F2;
                case 115:   return EKeyCode::F3;
                case 116:   return EKeyCode::F4;
                case 117:   return EKeyCode::F5;
                case 118:   return EKeyCode::F6;
                case 119:   return EKeyCode::F7;
                case 120:   return EKeyCode::F8;
                case 121:   return EKeyCode::F9;
                case 122:   return EKeyCode::F10;
                case 123:   return EKeyCode::F11;
                case 124:   return EKeyCode::F12;
                case 125:   return EKeyCode::F13;
                case 126:   return EKeyCode::F14;
                case 127:   return EKeyCode::F15;
                case 128:   return EKeyCode::F16;
                case 129:   return EKeyCode::F17;
                case 130:   return EKeyCode::F18;
                case 131:   return EKeyCode::F19;
                case 132:   return EKeyCode::F20;
                case 133:   return EKeyCode::F21;
                case 134:   return EKeyCode::F22;
                case 135:   return EKeyCode::F23;
                case 136:   return EKeyCode::F24;
                case 96:    return EKeyCode::KP0;
                case 97:    return EKeyCode::KP1;
                case 98:    return EKeyCode::KP2;
                case 99:    return EKeyCode::KP3;
                case 100:   return EKeyCode::KP4;
                case 101:   return EKeyCode::KP5;
                case 102:   return EKeyCode::KP6;
                case 103:   return EKeyCode::KP7;
                case 104:   return EKeyCode::KP8;
                case 105:   return EKeyCode::KP9;
                case 110:   return EKeyCode::KPDecimal;
                case 111:   return EKeyCode::KPDivide;
                case 106:   return EKeyCode::KPMultiply;
                case 109:   return EKeyCode::KPSubtract;
                case 107:   return EKeyCode::KPAdd;
                case 108:   return EKeyCode::KPEqual;
                case 160:   return EKeyCode::LeftShift;
                case 162:   return EKeyCode::LeftControl;
                case 164:   return EKeyCode::LeftAlt;
                case 161:   return EKeyCode::RightShift;
                case 163:   return EKeyCode::RightControl;
                case 165:   return EKeyCode::RightAlt;
            }
        }

        static int32 EMouseCodeToWindowsMouseCode(EMouseCode code)
        {
            CS_PROFILE_FN();

            switch (code)
            {
                case EMouseCode::ButtonLeft:   return 1;
                case EMouseCode::ButtonRight:  return 2;
                case EMouseCode::ButtonMiddle: return 4;
            }
        }

        static EMouseCode WindowsKeyToEMouseCode(int32 key)
        {
            CS_PROFILE_FN();

            switch (key)
            {
                case 1: return EMouseCode::ButtonLeft;
                case 2: return EMouseCode::ButtonRight;
                case 4: return EMouseCode::ButtonMiddle;
            }
        }
    }

    bool Input::IsKeyPressed(EKeyCode code)
    {
        CS_PROFILE_FN();

        int32 windowsKey = Utils::EKeyCodeToWindowsKeyCode(code);
        auto state = GetAsyncKeyState(windowsKey);
        return (state & 0x8000);
    }

    bool Input::IsMouseButtonPressed(EMouseCode button)
    {
        CS_PROFILE_FN();

        int32 windowsKey = Utils::EMouseCodeToWindowsMouseCode(button);
        auto state = GetAsyncKeyState(windowsKey);
        return (state & 0x8000);
    }

    EKeyCode Input::GetKeyPressed()
    {
        CS_PROFILE_FN();

        BYTE windowsKey;
        GetKeyboardState(&windowsKey);
        return Utils::WindowsKeyToEKeyCode((int32)windowsKey);
    }

    EMouseCode Input::GetMouseButtonPressed()
    {
        CS_PROFILE_FN();

        BYTE windowsKey;
        GetKeyboardState(&windowsKey);
        return Utils::WindowsKeyToEMouseCode((int32)windowsKey);
    }

    float2 Input::GetMousePosition()
    {
        CS_PROFILE_FN();

        POINT p;
        ::GetCursorPos(&p);
        return { (float32)p.x, (float32)p.y };
    }

    void Input::SetCursorMode(ECursorMode mode)
    {
        CS_PROFILE_FN();

        switch (mode)
        {
            case ECursorMode::Normal:
            {
                ::SetCursor(::LoadCursor(nullptr, IDC_ARROW));
                break;
            }
            case ECursorMode::Hidden:
            {
                ::SetCursor(nullptr);
                break;
            }
        }
    }

}