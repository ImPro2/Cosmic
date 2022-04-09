module;
#include "cspch.hpp"
#include <glad/glad.h>
module Cosmic.Impl.RendererAPI.OpenGL.OpenGLBuffer;

namespace Cosmic
{

    static GLenum EShaderDataTypeToOpenGLType(EShaderDataType type)
    {
        switch (type)
        {
            case EShaderDataType::Float:    return GL_FLOAT;
            case EShaderDataType::Float2:   return GL_FLOAT;
            case EShaderDataType::Float3:   return GL_FLOAT;
            case EShaderDataType::Float4:   return GL_FLOAT;
            case EShaderDataType::Mat3:     return GL_FLOAT;
            case EShaderDataType::Mat4:     return GL_FLOAT;
            case EShaderDataType::Int:      return GL_INT;
            case EShaderDataType::Int2:     return GL_INT;
            case EShaderDataType::Int3:     return GL_INT;
            case EShaderDataType::Int4:     return GL_INT;
            case EShaderDataType::Bool:     return GL_BOOL;
        }
    }

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

    void OpenGLVertexBuffer::SetLayout(const VertexBufferLayout& layout)
    {
        mLayout = layout;

        glBindBuffer(GL_ARRAY_BUFFER, mRendererID);

        uint32 index = 0;

        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                element.ComponentCount,
                EShaderDataTypeToOpenGLType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (GLvoid*)element.Offset
            );

            index++;
        }
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