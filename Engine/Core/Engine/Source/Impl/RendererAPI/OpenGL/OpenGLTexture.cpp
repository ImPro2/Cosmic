#include "Base/Macros.hpp"
#include "Renderer/Texture.hpp"
#include "cspch.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLErrors.hpp"

#include <glad/glad.h>
#include <stb_image.h>

CS_MODULE_LOG_INFO(Cosmic, Impl.RendererAPI.Opengl.OpenGLTexture);

namespace Cosmic
{
#if 0

    namespace Utils
    {

#if 0
        static GLenum ETextureWrapModeToOpenGLWrapMode(ETextureWrapMode wrapMode)
        {
            switch (wrapMode)
            {
                case ETextureWrapMode::ClampToEdge:    return GL_CLAMP_TO_EDGE;
                case ETextureWrapMode::ClampToBorder:  return GL_CLAMP_TO_BORDER;
                case ETextureWrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
                case ETextureWrapMode::Repeat:         return GL_REPEAT;
            }
        }

        static GLenum ETextureScalingFilterToOpenGLScalingFilter(ETextureScalingFilter scalingFilter)
        {
            switch (scalingFilter)
            {
                case ETextureScalingFilter::Nearest:              return GL_NEAREST;
                case ETextureScalingFilter::Linear:               return GL_LINEAR;
                case ETextureScalingFilter::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
                case ETextureScalingFilter::LinearMipmapNearest:  return GL_LINEAR_MIPMAP_NEAREST;
                case ETextureScalingFilter::NearestMipmapLinear:  return GL_NEAREST_MIPMAP_LINEAR;
                case ETextureScalingFilter::LineaerMipmapLinear:  return GL_LINEAR_MIPMAP_LINEAR;
            }
        }
        
        static GLenum ETextureFormatToOpenGLInternalFormat(ETextureFormat format)
        {
            switch (format)
            {
                case ETextureFormat::R8_Float:        return GL_R8;
                case ETextureFormat::R8_UInt:         return GL_R8UI;
                case ETextureFormat::R8_SInt:         return GL_R8I;
                case ETextureFormat::R16_Float:       return GL_R16F;
                case ETextureFormat::R16_UInt:        return GL_R16UI;
                case ETextureFormat::R16_SInt:        return GL_R16I;
                case ETextureFormat::R32_Float:       return GL_R32F;
                case ETextureFormat::R32_UInt:        return GL_R32UI;
                case ETextureFormat::R32_SInt:        return GL_R32I;
                case ETextureFormat::RG8_Float:       return GL_RG8;
                case ETextureFormat::RG8_UInt:        return GL_RG8UI;
                case ETextureFormat::RG8_SInt:        return GL_RG8I;
                case ETextureFormat::RG16_Float:      return GL_RG16F;
                case ETextureFormat::RG16_UInt:       return GL_RG16UI;
                case ETextureFormat::RG16_SInt:       return GL_RG16I;
                case ETextureFormat::RG32_Float:      return GL_RG32F;
                case ETextureFormat::RG32_UInt:       return GL_RG32UI;
                case ETextureFormat::RG32_SInt:       return GL_RG32I;
                case ETextureFormat::RGB8_Float:      return GL_RGB8;
                case ETextureFormat::RGB8_UInt:       return GL_RGB8UI;
                case ETextureFormat::RGB8_SInt:       return GL_RGB8I;
                case ETextureFormat::RGB16_Float:     return GL_RGB16F;
                case ETextureFormat::RGB16_UInt:      return GL_RGB16UI;
                case ETextureFormat::RGB16_SInt:      return GL_RGB16I;
                case ETextureFormat::RGB32_Float:     return GL_RGB32F;
                case ETextureFormat::RGB32_UInt:      return GL_RGB32UI;
                case ETextureFormat::RGB32_SInt:      return GL_RGB32I;
                case ETextureFormat::RGBA8_Float:     return GL_RGBA8;
                case ETextureFormat::RGBA8_UInt:      return GL_RGBA8UI;
                case ETextureFormat::RGBA8_SInt:      return GL_RGBA8I;
                case ETextureFormat::RGBA16_Float:    return GL_RGBA16F;
                case ETextureFormat::RGBA16_UInt:     return GL_RGBA16UI;
                case ETextureFormat::RGBA16_SInt:     return GL_RGBA16I;
                case ETextureFormat::RGBA32_Float:    return GL_RGBA32F;
                case ETextureFormat::RGBA32_UInt:     return GL_RGBA32UI;
                case ETextureFormat::RGBA32_SInt:     return GL_RGBA32I;
                case ETextureFormat::Depth16:         return GL_DEPTH_COMPONENT16;
                case ETextureFormat::Depth24:         return GL_DEPTH_COMPONENT24;
                case ETextureFormat::Depth32:         return GL_DEPTH_COMPONENT32;
                case ETextureFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
                case ETextureFormat::Depth32Stencil8: return GL_DEPTH32F_STENCIL8;
            }
        }

        static GLenum ETextureFormatToOpenGLDataFormat(ETextureFormat format)
        {
            switch (format)
            {
                case ETextureFormat::R8_Float:        return GL_RED;
                case ETextureFormat::R8_UInt:         return GL_RED_INTEGER;
                case ETextureFormat::R8_SInt:         return GL_RED_INTEGER;
                case ETextureFormat::R16_Float:       return GL_RED;
                case ETextureFormat::R16_UInt:        return GL_RED_INTEGER;
                case ETextureFormat::R16_SInt:        return GL_RED_INTEGER;
                case ETextureFormat::R32_Float:       return GL_RED;
                case ETextureFormat::R32_UInt:        return GL_RED_INTEGER;
                case ETextureFormat::R32_SInt:        return GL_RED_INTEGER;
                case ETextureFormat::RG8_Float:       return GL_RG;
                case ETextureFormat::RG8_UInt:        return GL_RG_INTEGER;
                case ETextureFormat::RG8_SInt:        return GL_RG_INTEGER;
                case ETextureFormat::RG16_Float:      return GL_RG;
                case ETextureFormat::RG16_UInt:       return GL_RG_INTEGER;
                case ETextureFormat::RG16_SInt:       return GL_RG_INTEGER;
                case ETextureFormat::RG32_Float:      return GL_RG;
                case ETextureFormat::RG32_UInt:       return GL_RG_INTEGER;
                case ETextureFormat::RG32_SInt:       return GL_RG_INTEGER;
                case ETextureFormat::RGB8_Float:      return GL_RGB;
                case ETextureFormat::RGB8_UInt:       return GL_RGB_INTEGER;
                case ETextureFormat::RGB8_SInt:       return GL_RGB_INTEGER;
                case ETextureFormat::RGB16_Float:     return GL_RGB;
                case ETextureFormat::RGB16_UInt:      return GL_RGB_INTEGER;
                case ETextureFormat::RGB16_SInt:      return GL_RGB_INTEGER;
                case ETextureFormat::RGB32_Float:     return GL_RGB;
                case ETextureFormat::RGB32_UInt:      return GL_RGB_INTEGER;
                case ETextureFormat::RGB32_SInt:      return GL_RGB_INTEGER;
                case ETextureFormat::RGBA8_Float:     return GL_RGBA;
                case ETextureFormat::RGBA8_UInt:      return GL_RGBA_INTEGER;
                case ETextureFormat::RGBA8_SInt:      return GL_RGBA_INTEGER;
                case ETextureFormat::RGBA16_Float:    return GL_RGBA;
                case ETextureFormat::RGBA16_UInt:     return GL_RGBA_INTEGER;
                case ETextureFormat::RGBA16_SInt:     return GL_RGBA_INTEGER;
                case ETextureFormat::RGBA32_Float:    return GL_RGBA;
                case ETextureFormat::RGBA32_UInt:     return GL_RGBA_INTEGER;
                case ETextureFormat::RGBA32_SInt:     return GL_RGBA_INTEGER;
                case ETextureFormat::Depth16:         return GL_DEPTH_COMPONENT16;
                case ETextureFormat::Depth24:         return GL_DEPTH_COMPONENT24;
                case ETextureFormat::Depth32:         return GL_DEPTH_COMPONENT32;
                case ETextureFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
                case ETextureFormat::Depth32Stencil8: return GL_DEPTH32F_STENCIL8;
            }
        }
    
        static GLenum ETextureFormatToOpenGLDataType(ETextureFormat format)
        {
            switch (format)
            {
                case ETextureFormat::R8_Float:
                case ETextureFormat::R16_Float:
                case ETextureFormat::R32_Float:
                case ETextureFormat::RG8_Float:
                case ETextureFormat::RG16_Float:
                case ETextureFormat::RG32_Float:
                case ETextureFormat::RGB8_Float:
                case ETextureFormat::RGB16_Float:
                case ETextureFormat::RGB32_Float:
                case ETextureFormat::RGBA8_Float:
                case ETextureFormat::RGBA16_Float:
                case ETextureFormat::RGBA32_Float:
                case ETextureFormat::Depth16:
                case ETextureFormat::Depth24:
                case ETextureFormat::Depth32:
                case ETextureFormat::Depth24Stencil8:
                case ETextureFormat::Depth32Stencil8:
                    return GL_UNSIGNED_BYTE;
                case ETextureFormat::R8_SInt:
                case ETextureFormat::R16_SInt:
                case ETextureFormat::R32_SInt:
                case ETextureFormat::RG8_SInt:
                case ETextureFormat::RG16_SInt:
                case ETextureFormat::RG32_SInt:
                case ETextureFormat::RGB8_SInt:
                case ETextureFormat::RGB16_SInt:
                case ETextureFormat::RGB32_SInt:
                case ETextureFormat::RGBA8_SInt:
                case ETextureFormat::RGBA16_SInt:
                case ETextureFormat::RGBA32_SInt:
                    return GL_INT;
                case ETextureFormat::R8_UInt:
                case ETextureFormat::R16_UInt:
                case ETextureFormat::R32_UInt:
                case ETextureFormat::RG8_UInt:
                case ETextureFormat::RG16_UInt:
                case ETextureFormat::RG32_UInt:
                case ETextureFormat::RGB8_UInt:
                case ETextureFormat::RGB16_UInt:
                case ETextureFormat::RGB32_UInt:
                case ETextureFormat::RGBA8_UInt:
                case ETextureFormat::RGBA16_UInt:
                case ETextureFormat::RGBA32_UInt:
                    return GL_UNSIGNED_INT;
            }
        }

        static ETextureAttachmentType ETextureFormatToETextureAttachmentType(ETextureFormat format)
        {
            switch (format)
            {
                case ETextureFormat::Depth16:         return ETextureAttachmentType::DepthAttachment;
                case ETextureFormat::Depth24:         return ETextureAttachmentType::DepthAttachment;
                case ETextureFormat::Depth32:         return ETextureAttachmentType::DepthAttachment;
                case ETextureFormat::Depth24Stencil8: return ETextureAttachmentType::DepthStencilAttachment;
                case ETextureFormat::Depth32Stencil8: return ETextureAttachmentType::DepthStencilAttachment;
            }

            return ETextureAttachmentType::ColorAttachment;
        }
#endif

    }

    OpenGLTexture2D::OpenGLTexture2D(const String& filePath, const Texture2DInfo& info)
        : Texture2D(filePath, info)
    {
        CS_PROFILE_FN();
#if 0

        int32  width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        uint8* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

        if (data)
        {
            mInfo.Width     = mInfo.Width  == 0 ? width  : mInfo.Width;
            mInfo.Height    = mInfo.Height == 0 ? height : mInfo.Height;
            GLenum wrap     = Utils::ETextureWrapModeToOpenGLWrapMode(mInfo.WrapMode);
            GLenum minScale = Utils::ETextureScalingFilterToOpenGLScalingFilter(mInfo.MinScalingFilter);
            GLenum magScale = Utils::ETextureScalingFilterToOpenGLScalingFilter(mInfo.MagScalingFilter);

            if (channels == 4)
            {
                mInternalFormat = GL_RGBA8;
                mDataFormat     = GL_RGBA;
            }
            else if (channels == 3)
            {
                mInternalFormat = GL_RGB8;
                mDataFormat     = GL_RGB;
            }

            GL_CALL(glGenTextures(1, &mRendererID));
            GL_CALL(glBindTexture(GL_TEXTURE_2D, mRendererID));

            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minScale));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magScale));

            GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, width, height, 0, mDataFormat, GL_UNSIGNED_BYTE, data));
            GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

            stbi_image_free(data);
        }
#endif
    }

    OpenGLTexture2D::OpenGLTexture2D(const Texture2DInfo& info)
        : Texture2D(info)
    {
#if 0
        CS_PROFILE_FN();

        GLenum wrap      = Utils::ETextureWrapModeToOpenGLWrapMode(mInfo.WrapMode);
        GLenum minScale  = Utils::ETextureScalingFilterToOpenGLScalingFilter(mInfo.MinScalingFilter);
        GLenum magScale  = Utils::ETextureScalingFilterToOpenGLScalingFilter(mInfo.MagScalingFilter);
        mInternalFormat  = Utils::ETextureFormatToOpenGLInternalFormat(mInfo.Format);
        mDataFormat      = Utils::ETextureFormatToOpenGLDataFormat(mInfo.Format);
        mDataType        = Utils::ETextureFormatToOpenGLDataType(mInfo.Format);

        GL_CALL(glGenTextures(1, &mRendererID));
        GL_CALL(glBindTexture (GL_TEXTURE_2D, mRendererID));
        GL_CALL(glActiveTexture(GL_TEXTURE0));

        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minScale));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magScale));

        //if (info.Format != ETextureFormat::R32_SInt)
            GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, mInfo.Width, mInfo.Height, 0, mDataFormat, mDataType, nullptr));
        //else
        //    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, mInfo.Width, mInfo.Height, 0, GL_RED_INTEGER, GL_INT, nullptr));
#endif
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        CS_PROFILE_FN();

        GL_CALL(glDeleteTextures(1, &mRendererID));
    }

    void OpenGLTexture2D::SetData(void* data, uint32 size)
    {
        CS_PROFILE_FN();

        //GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, mInfo.Width, mInfo.Height, 0, mDataFormat, GL_UNSIGNED_BYTE, data));
        GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
    }

    void OpenGLTexture2D::Bind(uint32 slot) const
    {
        CS_PROFILE_FN();

        GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, mRendererID));
    }
#endif

}
