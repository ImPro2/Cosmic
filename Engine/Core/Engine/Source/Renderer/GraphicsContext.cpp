#include "cspch.hpp"
#include "GraphicsContext.hpp"

#include "Renderer/RendererAPI.hpp"
#include "Impl/RendererAPI/OpenGL/OpenGLGraphicsContext.hpp"

namespace Cosmic
{

    Ref<GraphicsContext> CreateGraphicsContext(void* window)
    {
        CS_PROFILE_FN();

        switch (RendererAPI::Get())
        {
            //case ERendererAPI::OpenGL: return CreateRef<OpenGLGraphicsContext>((GLFWwindow*)window); break;
        }
        return {};
    }

}
