module;
#include "cspch.hpp"
#include <initializer_list>
module Cosmic.Renderer.Buffer;

import Cosmic.Renderer.RendererAPI;
import Cosmic.Impl.RendererAPI.OpenGL.OpenGLBuffer;

namespace Cosmic
{

    static uint32 EShaderDataTypeToSize(EShaderDataType type)
    {
        switch (type)
        {
            case EShaderDataType::Float:  return 4;
            case EShaderDataType::Float2: return 4 * 2;
            case EShaderDataType::Float3: return 4 * 3;
            case EShaderDataType::Float4: return 4 * 4;
            case EShaderDataType::Int:    return 4;
            case EShaderDataType::Int2:   return 4 * 2;
            case EShaderDataType::Int3:   return 4 * 3;
            case EShaderDataType::Int4:   return 4 * 4;
            case EShaderDataType::Mat2:   return 4 * 2 * 2;
            case EShaderDataType::Mat3:   return 4 * 3 * 3;
            case EShaderDataType::Mat4:   return 4 * 4 * 4;
            case EShaderDataType::Bool:   return 1;
        }
    }

    static uint32 EShaderDataTypeComponentCount(EShaderDataType type)
    {
        switch (type)
        {
            case EShaderDataType::Float:  return 1;
            case EShaderDataType::Float2: return 2;
            case EShaderDataType::Float3: return 3;
            case EShaderDataType::Float4: return 4;
            case EShaderDataType::Int:    return 1;
            case EShaderDataType::Int2:   return 2;
            case EShaderDataType::Int3:   return 3;
            case EShaderDataType::Int4:   return 4;
            case EShaderDataType::Mat2:   return 2;
            case EShaderDataType::Mat3:   return 3;
            case EShaderDataType::Mat4:   return 4;
            case EShaderDataType::Bool:   return 1;
        }
    }

    // vertex buffer layout

    VertexBufferElement::VertexBufferElement(EShaderDataType type, bool normalized)
        : ComponentCount(EShaderDataTypeComponentCount(type)),
          Type(type),
          Normalized(normalized),
          Offset(0),
          Size(EShaderDataTypeToSize(type))
    {
    }

    VertexBufferLayout::VertexBufferLayout(std::initializer_list<VertexBufferElement> elements)
        : mElements(elements)
    {
        CalculateOffsetsAndStride();
    }

    void VertexBufferLayout::CalculateOffsetsAndStride()
    {
        uint32 offset = 0;
        mStride       = 0;

        for (auto& element : mElements)
        {
            element.Offset = offset;

            offset  += element.Size;
            mStride += element.Size;
        }
    }

    // vertex buffer

    VertexBuffer::VertexBuffer(void* vertices, uint32 size, EBufferUsage usage)
        : mSize(size), mUsage(usage)
    {
    }

    // index buffer

    IndexBuffer::IndexBuffer(uint32* indices, uint32 count, EBufferUsage usage)
        : mCount(count), mUsage(usage)
    {
    }

    // creation functions

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