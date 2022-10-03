module;
#include "cspch.hpp"
#include <GLFW/glfw3.h>
module Cosmic.Renderer.GraphicsContext;

import Cosmic.Renderer.RendererAPI;
import Cosmic.Impl.RendererAPI.OpenGL.OpenGLGraphicsContext;

namespace Cosmic
{

    Scope<GraphicsContext> CreateGraphicsContext(void* window)
    {
        CS_PROFILE_FN();

        switch (RendererAPI::Get())
        {
            case ERendererAPI::OpenGL: return CreateScope<OpenGLGraphicsContext>((GLFWwindow*)window); break;
        }
    }

}
