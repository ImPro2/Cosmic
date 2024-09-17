#pragma once
#include <glad/glad.h>

#include "Renderer/Framebuffer.hpp"
#include "Base/Base.hpp"

namespace Cosmic
{

    class OpenGLFramebuffer : public Framebuffer
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