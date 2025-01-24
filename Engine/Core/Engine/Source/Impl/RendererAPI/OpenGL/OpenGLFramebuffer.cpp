#include "cspch.hpp"

#include "Renderer/Texture.hpp"
#include "Base/Macros.hpp"
#include "Renderer/Framebuffer.hpp"
#include "OpenGLFramebuffer.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLErrors.hpp"

#include <glad/glad.h>
#include <memory>

CS_MODULE_LOG_INFO(Cosmic, Impl.RendererAPI.OpenGL.OpenGLFramebuffer);

namespace Cosmic
{

    namespace Utils
    {

        static GLenum IndexToOpenGLColorAttachmentIndex(int32 index)
        {
            return GL_COLOR_ATTACHMENT0 + index;
        }

        static GLenum ETextureAttachmentTypeToOpenGLTextureAttachment(EAttachmentType attachmentType, int32 colorAttachmentIndex)
        {
            switch (attachmentType)
            {
                case EAttachmentType::ColorAttachment:        return IndexToOpenGLColorAttachmentIndex(colorAttachmentIndex);
                case EAttachmentType::DepthAttachment:        return GL_DEPTH_ATTACHMENT;
                case EAttachmentType::DepthStencilAttachment: return GL_DEPTH_STENCIL_ATTACHMENT;
            }
        }

    }

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferInfo& info)
        : Framebuffer(info)
    {
        CS_PROFILE_FN();

        CreateAttachments();
        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        CS_PROFILE_FN();

#if 0
        GL_CALL(glDeleteFramebuffers(1, &mRendererID));
        mTextures.clear();
#endif
    }

    void OpenGLFramebuffer::Invalidate()
    {
#if 0
        CS_PROFILE_FN();

        if (mRendererID)
        {
            GL_CALL(glDeleteFramebuffers(1, &mRendererID));
            mTextures.clear();

            CreateAttachments();
        }

        GL_CALL(glGenFramebuffers(1, &mRendererID));
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mRendererID));

        int32 colorAttachmentIndex = 0;

        for (const Ref<Texture2D>& texture : mTextures)
        {
            GLenum attachmentType    = Utils::ETextureAttachmentTypeToOpenGLTextureAttachment(texture->GetAttachmentType(), colorAttachmentIndex);
            GLenum textureRendererID = texture->GetRendererID();

            GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, textureRendererID, 0));

            if (texture->GetAttachmentType() == ETextureAttachmentType::ColorAttachment)
                colorAttachmentIndex++;
        }

        int32 colorAttachmentCount = colorAttachmentIndex;
        colorAttachmentIndex = 0;

        GLenum* colorAttachments = (GLenum*)alloca(colorAttachmentCount * sizeof(GLenum));

        for (const Ref<Texture2D>& texture : mTextures)
        {
            if (texture->GetAttachmentType() != ETextureAttachmentType::ColorAttachment)
                continue;

            colorAttachments[colorAttachmentIndex] = Utils::IndexToOpenGLColorAttachmentIndex(colorAttachmentIndex);
            colorAttachmentIndex++;
        }

        GLenum colorAttachmentsGl[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        GL_CALL(glDrawBuffers(2, colorAttachmentsGl));

        //if (colorAttachmentCount > 0)
        //    GL_CALL(glDrawBuffers(colorAttachmentCount, colorAttachments));
        //else
        //    GL_CALL(glDrawBuffer(GL_NONE));
        
        int32 result;
        GL_CALL(result = glCheckFramebufferStatus(GL_FRAMEBUFFER));
        CS_ASSERT(result == GL_FRAMEBUFFER_COMPLETE, "Failed to create framebuffer.");
        
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
#endif
    }

    void OpenGLFramebuffer::Resize(uint32 width, uint32 height)
    {
#if 0
        mInfo.Width = width;
        mInfo.Height = height;

        Invalidate();
#endif
    }

    void OpenGLFramebuffer::Bind()
    {
        CS_PROFILE_FN();

#if 0
        if (mInfo.SwapChainTarget)
            GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        else
        {
            GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mRendererID));
            GL_CALL(glViewport(0, 0, mInfo.Width, mInfo.Height));
        }
#endif
    }

    void OpenGLFramebuffer::Unbind()
    {
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

#if 0
    uint32 OpenGLFramebuffer::GetColorAttachmentRendererID()
    {
        for (const Ref<Texture2D>& texture : mTextures)
        {
            if (texture->GetAttachmentType() == ETextureAttachmentType::ColorAttachment)
                return texture->GetRendererID();
        }
    }
#endif

    void OpenGLFramebuffer::ClearAttachment(uint32 attachmentIndex, int32 value)
    {
#if 0
        const Ref<OpenGLTexture2D>& texture = mTextures[attachmentIndex].As<OpenGLTexture2D>();

        GL_CALL(glClearTexImage(texture->GetRendererID(), 0, texture->GetDataFormat(), GL_INT, &value));
#endif
    }

    int32 OpenGLFramebuffer::ReadPixel(uint32 attachmentIndex, int2 pos)
    {
#if 0
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mRendererID));
        GL_CALL(glReadBuffer(Utils::IndexToOpenGLColorAttachmentIndex(attachmentIndex)));

        const Ref<OpenGLTexture2D> attachment = mTextures[attachmentIndex].As<OpenGLTexture2D>();

        float32 pixelData;

        GL_CALL(glReadPixels(pos.x, pos.y, 1, 1, attachment->GetDataFormat(), attachment->GetDataType(), &pixelData));

        return (int32)pixelData;
#endif
        return 0;
    }

    void OpenGLFramebuffer::CreateAttachments()
    {
#if 0

        for (const auto& attachment : mInfo.AttachmentsInfo.Attachments)
        {
            Texture2DInfo textureInfo = Texture2DInfo({ mInfo.Width, mInfo.Height }, attachment.WrapMode, attachment.MinScalingFilter, attachment.MagScalingFilter, attachment.Format);
            mTextures.emplace_back(CreateTexture2D(textureInfo));
        }
#endif
    }

}
