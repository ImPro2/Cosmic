module;
#include "cspch.hpp"
export module Cosmic.Impl.RendererAPI.OpenGL.OpenGLErrors;

import Cosmic.Base;

namespace Cosmic
{

    export struct OpenGLError
    {
        uint32      ErrorCode;
        const char* Description;
    };

    export void                OpenGLClearErrors();
    export Vector<OpenGLError> OpenGLCheckErrors();

}