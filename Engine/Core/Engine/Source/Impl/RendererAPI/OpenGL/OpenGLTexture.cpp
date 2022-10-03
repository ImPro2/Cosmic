module;
#include "cspch.hpp"
#include <glad/glad.h>
#include <stb_image.h>
module Cosmic.Impl.RendererAPI.OpenGL.OpenGLTexture;

namespace Cosmic
{

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

    OpenGLTexture2D::OpenGLTexture2D(const String& filePath, ETextureWrapMode wrapMode, ETextureScalingFilter min, ETextureScalingFilter mag)
        : Texture2D(filePath, wrapMode, min, mag)
    {
        CS_PROFILE_FN();

        int32  width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        uint8* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

        if (data)
        {
            mIsLoaded        = true;
            mWidth           = width;
            mHeight          = height;
            GLenum wrap      = ETextureWrapModeToOpenGLWrapMode(wrapMode);
            GLenum minScale  = ETextureScalingFilterToOpenGLScalingFilter(min);
            GLenum magScale = ETextureScalingFilterToOpenGLScalingFilter(mag);

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

            glGenTextures(1, &mRendererID);
            glBindTexture(GL_TEXTURE_2D, mRendererID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minScale);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magScale);

            glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, width, height, 0, mDataFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(data);
        }
    }

    OpenGLTexture2D::OpenGLTexture2D(uint32 width, uint32 height, ETextureWrapMode wrapMode, ETextureScalingFilter min, ETextureScalingFilter mag)
        : Texture2D(width, height, wrapMode, min, mag)
    {
        CS_PROFILE_FN();

        mInternalFormat  = GL_RGBA8;
        mDataFormat      = GL_RGBA;
        GLenum wrap      = ETextureWrapModeToOpenGLWrapMode(wrapMode);
        GLenum minScale  = ETextureScalingFilterToOpenGLScalingFilter(min);
        GLenum magScale = ETextureScalingFilterToOpenGLScalingFilter(mag);

        glGenTextures(1, &mRendererID);
        glBindTexture (GL_TEXTURE_2D, mRendererID);
        glActiveTexture(GL_TEXTURE0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minScale);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magScale);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        CS_PROFILE_FN();

        glDeleteTextures(1, &mRendererID);
    }

    void OpenGLTexture2D::SetData(void* data, uint32 size)
    {
        CS_PROFILE_FN();

        uint32 bpp = mDataFormat == GL_RGBA ? 4 : 3;
        glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, mWidth, mHeight, 0, mDataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void OpenGLTexture2D::Bind(uint32 slot) const
    {
        CS_PROFILE_FN();

        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, mRendererID);
    }

}
