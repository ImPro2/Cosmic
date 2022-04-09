module;
#include "cspch.hpp"
#include <glad/glad.h>
export module Cosmic.Impl.RendererAPI.OpenGL.OpenGLBuffer;

import Cosmic.Renderer.Buffer;
import Cosmic.Base;

namespace Cosmic
{

    export class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(void* vertices, uint32 size, EBufferUsage usage);
        virtual ~OpenGLVertexBuffer();

        void SetData(const void* data, uint32 size, uint32 offset) override;
        void SetLayout(const VertexBufferLayout& layout)           override;

        void Bind()   const override;
        void Unbind() const override;

    private:
        uint32 mRendererID;
    };

    export class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32* indices, uint32 count, EBufferUsage usage);
        virtual ~OpenGLIndexBuffer();

        void Bind()   const override;
        void Unbind() const override;

    private:
        uint32 mRendererID;
    };

}