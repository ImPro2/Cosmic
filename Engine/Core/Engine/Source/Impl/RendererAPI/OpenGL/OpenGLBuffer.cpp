module;
#include "cspch.hpp"
#include <glad/glad.h>
module Cosmic.Impl.RendererAPI.OpenGL.OpenGLBuffer;

namespace Cosmic
{

    static GLenum EBufferUsageToOpenGLBufferUsage(EBufferUsage usage)
    {
        switch (usage)
        {
            case EBufferUsage::Static:  return GL_STATIC_DRAW;
            case EBufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
            case EBufferUsage::Stream:  return GL_STREAM_DRAW;
        }

        CS_BREAK();
        return -1;
    }

    // Vertex Buffer

    OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, uint32 size, EBufferUsage usage)
        : VertexBuffer(vertices, size, usage)
    {
        glGenBuffers(1, &mRendererID);
        glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, EBufferUsageToOpenGLBufferUsage(usage));

        if (vertices)
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, (const void*)vertices);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &mRendererID);
    }

    void OpenGLVertexBuffer::SetData(const void* data, uint32 size, uint32 offset)
    {
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // Index Buffer

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32* indices, uint32 count, EBufferUsage usage)
        : IndexBuffer(indices, count, usage)
    {
        glGenBuffers(1, &mRendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * count, (const void*)indices, EBufferUsageToOpenGLBufferUsage(usage));
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &mRendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}