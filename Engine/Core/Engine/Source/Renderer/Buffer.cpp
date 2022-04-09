module;
#include "cspch.hpp"
module Cosmic.Renderer.Buffer;

import Cosmic.Renderer.RendererAPI;
import Cosmic.Impl.RendererAPI.OpenGL.OpenGLBuffer;

namespace Cosmic
{

    VertexBuffer::VertexBuffer(void* vertices, uint32 size, EBufferUsage usage)
        : mSize(size), mUsage(usage)
    {
    }

    IndexBuffer::IndexBuffer(uint32* indices, uint32 count, EBufferUsage usage)
        : mCount(count), mUsage(usage)
    {
    }

    Ref<VertexBuffer> CreateVertexBuffer(void* vertices, uint32 size, EBufferUsage usage)
    {
        switch (RendererAPI::Get())
        {
            case ERendererAPI::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size, usage); break;
        }
    }

    Ref<IndexBuffer>  CreateIndexBuffer(uint32* indices, uint32 count, EBufferUsage usage)
    {
        switch (RendererAPI::Get())
        {
            case ERendererAPI::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count, usage); break;
        }
    }

}