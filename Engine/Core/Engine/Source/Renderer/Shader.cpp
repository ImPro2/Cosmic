module;
#include "cspch.hpp"
module Cosmic.Renderer.Shader;

import Cosmic.Renderer.RendererAPI;
import Cosmic.Impl.RendererAPI.OpenGL.OpenGLShader;

namespace Cosmic
{

    Shader::Shader(const String& filePath)
        : mFilePath(filePath), mName("")
    {
        CS_PROFILE_FN();
    }

    Shader::Shader(const String& name, const String& vertexSrc, const String& fragmentSrc)
        : mName(name)
    {
        CS_PROFILE_FN();
    }

    Ref<Shader> CreateShader(const String& filePath)
    {
        CS_PROFILE_FN();

        switch (RendererAPI::Get())
        {
            case ERendererAPI::OpenGL: return CreateRef<OpenGLShader>(filePath);
        }
    }

    Ref<Shader> CreateShader(const String& name, const String& vertexSrc, const String& fragmentSrc)
    {
        CS_PROFILE_FN();

        switch (RendererAPI::Get())
        {
            case ERendererAPI::OpenGL: return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }
    }

}
