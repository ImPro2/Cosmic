module;
#include "cspch.hpp"
#include <glad/glad.h>
export module Cosmic.Impl.RendererAPI.OpenGL.OpenGLFramebuffer;

import Cosmic.Renderer.Framebuffer;
import Cosmic.Base;

namespace Cosmic
{

    export class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferInfo& info);
        ~OpenGLFramebuffer();

    public:
        void Invalidate() override;
        void Resize(uint32 width, uint32 height) override;
        void Bind() override;
        void Unbind() override;
        uint32 GetColorAttachmentRendererID() override;

    private:
        GLenum mRendererID;
        GLenum mColorAttachmentRendererID;
    };

}