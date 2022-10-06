module;
#include "cspch.hpp"
module Cosmic.Renderer.Framebuffer;

import Cosmic.Renderer.RendererAPI;
import Cosmic.Impl.RendererAPI.OpenGL.OpenGLFramebuffer;

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