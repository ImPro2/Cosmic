#include "cspch.hpp"
#include "Buffer.hpp"
#include <initializer_list>

#include "Renderer/RendererAPI.hpp"
#include "Impl/RendererAPI/OpenGL/OpenGLBuffer.hpp"

namespace Cosmic
{

    static uint32 EShaderDataTypeToSize(EShaderDataType type)
    {
        switch (type)
        {
            case EShaderDataType::Float:  return sizeof(float32) * 1;
            case EShaderDataType::Float2: return sizeof(float32) * 2;
            case EShaderDataType::Float3: return sizeof(float32) * 3;
            case EShaderDataType::Float4: return sizeof(float32) * 4;
            case EShaderDataType::Int:    return sizeof(int32) * 1;
            case EShaderDataType::Int2:   return sizeof(int32) * 2;
            case EShaderDataType::Int3:   return sizeof(int32) * 3;
            case EShaderDataType::Int4:   return sizeof(int32) * 4;
            case EShaderDataType::Mat2:   return sizeof(float32) * 2 * 2;
            case EShaderDataType::Mat3:   return sizeof(float32) * 3 * 3;
            case EShaderDataType::Mat4:   return sizeof(float32) * 4 * 4;
            case EShaderDataType::Bool:   return sizeof(bool);
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
        CS_PROFILE_FN();
    }

    VertexBufferLayout::VertexBufferLayout(std::initializer_list<VertexBufferElement> elements)
        : mElements(elements)
    {
        CS_PROFILE_FN();

        CalculateOffsetsAndStride();
    }

    void VertexBufferLayout::CalculateOffsetsAndStride()
    {
        CS_PROFILE_FN();

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
