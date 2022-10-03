module;
#include "cspch.hpp"
#include <initializer_list>
export module Cosmic.Renderer.Buffer;

import Cosmic.Base;
import Cosmic.Renderer.Shader;

namespace Cosmic
{

    export struct VertexBufferElement
    {
        uint32          ComponentCount;
        EShaderDataType Type;
        bool            Normalized;
        uint32          Offset;
        uint32          Size; // NOT stride; just the size of the attribute.

        VertexBufferElement() = default;
        VertexBufferElement(EShaderDataType type, bool normalized = false);
    };

    export class VertexBufferLayout
    {
    public:
        VertexBufferLayout() = default;
        VertexBufferLayout(std::initializer_list<VertexBufferElement> elements);

    public:
        uint32                             GetStride()      const { return mStride;           }
        const Vector<VertexBufferElement>& GetElements()    const { return mElements;         }

        Vector<VertexBufferElement>::iterator       begin()       { return mElements.begin(); }
        Vector<VertexBufferElement>::iterator       end()         { return mElements.end();   }
        Vector<VertexBufferElement>::const_iterator begin() const { return mElements.begin(); }
        Vector<VertexBufferElement>::const_iterator end()   const { return mElements.end();   }

    private:
        void CalculateOffsetsAndStride();

    private:
        Vector<VertexBufferElement> mElements;
        uint32                      mStride;
    };

    export enum class EBufferUsage
    {
        Static,  // The data store contents will be modified once and used many times.
        Dynamic, // The data store contents will be modified repeatedly and used many times.
        Stream   // The data store contents will be modified once and used at most a few times.
    };

    export class VertexBuffer
    {
    public:
        VertexBuffer(void* vertices, uint32 size, EBufferUsage usage);
        virtual ~VertexBuffer() = default;

        virtual void SetData(const void* data, uint32 size, uint32 offset) = 0;
        virtual void SetLayout(const VertexBufferLayout& layout)           = 0;

    public:
        virtual void Bind()   const = 0;
        virtual void Unbind() const = 0;

    public:
        uint32             GetSize()   const { return mSize;   }
        EBufferUsage       GetUsage()  const { return mUsage;  }
        VertexBufferLayout GetLayout() const { return mLayout; }

    protected:
        uint32             mSize;
        EBufferUsage       mUsage;
        VertexBufferLayout mLayout;
    };

    export class IndexBuffer
    {
    public:
        IndexBuffer(uint32* indices, uint32 count, EBufferUsage usage);
        virtual ~IndexBuffer() = default;

        virtual void Bind()   const = 0;
        virtual void Unbind() const = 0;

        uint32 GetCount()       const { return mCount; }
        EBufferUsage GetUsage() const { return mUsage; }

    protected:
        uint32 mCount;
        EBufferUsage mUsage;
    };

    export Ref<VertexBuffer> CreateVertexBuffer(void* vertices, uint32 size, EBufferUsage usage = EBufferUsage::Static);
    export Ref<IndexBuffer>  CreateIndexBuffer(uint32* indices, uint32 count, EBufferUsage usage = EBufferUsage::Static);

}
