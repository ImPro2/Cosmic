#pragma once
#include "Base/Base.hpp"

namespace Cosmic
{

    struct OpenGLError
    {
        uint32      ErrorCode;
        const char* Description;
    };

    void                OpenGLClearErrors();
    Vector<OpenGLError> OpenGLCheckErrors();

}
