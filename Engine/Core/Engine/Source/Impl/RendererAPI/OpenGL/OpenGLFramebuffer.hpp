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
        void ClearAttachment(uint32 attachmentIndex, int32 value) override;
        int32 ReadPixel(uint32 attachmentIndex, int2 pos) override;

    public:
        void Bind() override;
        void Unbind() override;
        uint32 GetColorAttachmentRendererID() override;

    private:
        void CreateAttachments();

    private:
        GLenum mRendererID = 0;
    };

}
