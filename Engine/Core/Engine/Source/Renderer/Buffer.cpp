#include "cspch.hpp"
#include "Buffer.hpp"
#include <initializer_list>

#include "Renderer/RendererAPI.hpp"
#include "Impl/RendererAPI/OpenGL/OpenGLBuffer.hpp"

namespace Cosmic
{

    // vertex buffer

    VertexBuffer::VertexBuffer(void* vertices, uint32 size, EBufferUsage usage)
        : mSize(size), mUsage(usage)
    {
        CS_PROFILE_FN();
    }

    // index buffer

    IndexBuffer::IndexBuffer(uint32* indices, uint32 count, EBufferUsage usage)
        : mCount(count), mUsage(usage)
    {
        CS_PROFILE_FN();
    }

    // creation functions

    Ref<VertexBuffer> CreateVertexBuffer(void* vertices, uint32 size, EBufferUsage usage)
    {
        CS_PROFILE_FN();

        switch (RendererAPI::Get())
        {
            case ERendererAPI::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size, usage); break;
        }
    }

    Ref<IndexBuffer>  CreateIndexBuffer(uint32* indices, uint32 count, EBufferUsage usage)
    {
        CS_PROFILE_FN();

        switch (RendererAPI::Get())
        {
            case ERendererAPI::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count, usage); break;
        }
    }

}
