module;
#include "cspch.hpp"
#include <glad/glad.h>
module Cosmic.Impl.RendererAPI.OpenGL.OpenGLFramebuffer;

CS_MODULE_LOG_INFO(Cosmic, Impl.RendererAPI.OpenGL.OpenGLFramebuffer);

import Cosmic.App.Log;

namespace Cosmic
{

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferInfo& info)
        : Framebuffer(info)
    {
        CS_PROFILE_FN();

        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        CS_PROFILE_FN();

        glDeleteFramebuffers(1, &mRendererID);
    }

    void OpenGLFramebuffer::Invalidate()
    {
        CS_PROFILE_FN();

        glGenFramebuffers(1, &mRendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);

        glGenTextures(1, &mColorAttachmentRendererID);
        glBindTexture(GL_TEXTURE_2D, mColorAttachmentRendererID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mInfo.Width, mInfo.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachmentRendererID, 0);

        //GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        //glDrawBuffers(1, drawBuffers);

        CS_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Unable to create framebuffer");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(uint32 width, uint32 height)
    {
        mInfo.Width = width;
        mInfo.Height = height;
        Invalidate();
    }

    void OpenGLFramebuffer::Bind()
    {
        CS_PROFILE_FN();

        if (mInfo.SwapChainTarget)
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
            glViewport(0, 0, mInfo.Width, mInfo.Height);
        }
    }

    void OpenGLFramebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    uint32 OpenGLFramebuffer::GetColorAttachmentRendererID()
    {
        return (uint32)mColorAttachmentRendererID;
    }

}