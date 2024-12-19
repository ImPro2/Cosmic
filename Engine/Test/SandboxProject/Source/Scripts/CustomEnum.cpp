#include "CustomEnum.hpp"

using namespace Cosmic;

extern "C"
{

    CS_DLLEXPORT const char* ECustomEnumToStr(ECustomEnum value)
    {
        switch (value)
        {
			case ECustomEnum::SomeValue:      return "SomeValue";
			case ECustomEnum::SomeOtherValue: return "SomeOtherValue";
            case ECustomEnum::Last:           return "Last";
        }

        return "None";
    }

    CS_DLLEXPORT ECustomEnum ECustomEnumFromStr(const char* str)
    {
        if (strcmp(str, "SomeValue") == 0)
            return ECustomEnum::SomeValue;
        else if (strcmp(str, "SomeOtherValue") == 0)
            return ECustomEnum::SomeOtherValue;

        return ECustomEnum::None;
    }

}