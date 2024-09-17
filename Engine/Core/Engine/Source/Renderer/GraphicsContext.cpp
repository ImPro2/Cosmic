#include "cspch.hpp"
#include "GraphicsContext.hpp"

#include "Renderer/RendererAPI.hpp"
#include "Impl/RendererAPI/OpenGL/OpenGLGraphicsContext.hpp"

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
