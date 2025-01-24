#pragma once
#include <glad/glad.h>

#include "Renderer/Texture.hpp"
#include "Base/Base.hpp"

namespace Cosmic
{

#if 0
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const String& filePath, const Texture2DInfo& info);
        OpenGLTexture2D(const Texture2DInfo& info);
        ~OpenGLTexture2D();

        void SetData(void* data, uint32 size)         override;
        void Bind(uint32 slot)                  const override;

    public:

    public:
        GLenum GetInternalFormat() const { return mInternalFormat; }
        GLenum GetDataFormat()     const { return mDataFormat;     }
        GLenum GetDataType()       const { return mDataType;       }

    protected:
        GLenum mRendererID;
        GLenum mInternalFormat, mDataFormat, mDataType;
    };
#endif


}
