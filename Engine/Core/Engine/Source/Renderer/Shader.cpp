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
    }

    Shader::Shader(const String& name, const String& vertexSrc, const String& fragmentSrc)
        : mName(name)
    {
    }

    Ref<Shader> CreateShader(const String& filePath)
    {
        switch (RendererAPI::Get())
        {
            case ERendererAPI::OpenGL: return CreateRef<OpenGLShader>(filePath);
        }
    }

    Ref<Shader> CreateShader(const String& name, const String& vertexSrc, const String& fragmentSrc)
    {
        switch (RendererAPI::Get())
        {
            case ERendererAPI::OpenGL: return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }
    }

}