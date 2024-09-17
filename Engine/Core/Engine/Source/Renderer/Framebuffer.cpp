#include "cspch.hpp"
#include "Framebuffer.hpp"

#include "Renderer/RendererAPI.hpp"
#include "Impl/RendererAPI/OpenGL/OpenGLFramebuffer.hpp"

namespace Cosmic
{

    Framebuffer::Framebuffer(const FramebufferInfo& info)
        : mInfo(info)
    {
        CS_PROFILE_FN();
    }

    Ref<Framebuffer> CreateFramebuffer(const FramebufferInfo& info)
    {
        switch (RendererAPI::Get())
        {
            case ERendererAPI::OpenGL: return CreateRef<OpenGLFramebuffer>(info);
        }
    }

}