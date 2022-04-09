module;
#include "cspch.hpp"
#include <glad/glad.h>
module Cosmic.Impl.RendererAPI.OpenGL.OpenGLErrors;

CS_MODULE_LOG_INFO(Cosmic, Cosmic.Impl.RendererAPI.OpenGL.OpenGLErrors);

namespace Cosmic
{

    static const char* OpenGLErrorCodeToString(uint32 errorCode)
    {
        switch (errorCode)
        {
            case GL_INVALID_ENUM:      return "Invalid Enum";      break;
            case GL_INVALID_VALUE:     return "Invalid Value";     break;
            case GL_INVALID_OPERATION: return "Invalid Operation"; break;
            case GL_OUT_OF_MEMORY:     return "Out Of Memory";     break;
        }

        CS_BREAK();
        return "";
    }

    void OpenGLClearErrors()
    {
        while (glGetError() != GL_NO_ERROR);
    }

    Vector<OpenGLError> OpenGLCheckErrors()
    {
        Vector<OpenGLError> errors;

        while (uint32 errorCode = glGetError())
        {
            errors.emplace_back(errorCode, OpenGLErrorCodeToString(errorCode));
        }

        return errors;
    }

}