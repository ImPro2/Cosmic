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

#ifdef CS_DEBUG
    #define GL_CALL(fn)                                                 \
    do                                                                  \
    {                                                                   \
        OpenGLClearErrors();                                            \
        fn;                                                             \
        Vector<OpenGLError> errors = OpenGLCheckErrors();               \
        for (auto [errCode, errDesc] : errors)                          \
            CS_LOG_ERROR("OpenGL Error {}: errDesc", errCode, errDesc); \
        if (!errors.empty())                                            \
            CS_BREAK();                                                 \
    } while(0)
#else
    #define GL_CALL(fn)
#endif
