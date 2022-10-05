module;
#include "cspch.hpp"
#include <glad/glad.h>
export module Cosmic.Impl.RendererAPI.OpenGL.OpenGLTexture;

import Cosmic.Renderer.Texture;

namespace Cosmic
{

    export class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const String& filePath, ETextureWrapMode wrapMode, ETextureScalingFilter min, ETextureScalingFilter mag);
        OpenGLTexture2D(uint32 width, uint32 height, ETextureWrapMode wrapMode, ETextureScalingFilter min, ETextureScalingFilter mag);
        ~OpenGLTexture2D();

        void SetData(void* data, uint32 size)         override;
        void Bind(uint32 slot)                  const override;

    public:
        uint32 GetRendererID() const override { return (uint32)mRendererID; }

    protected:
        GLenum mRendererID;
        GLenum mInternalFormat, mDataFormat;
    };


}
