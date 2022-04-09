module;
#include "cspch.hpp"
export module Cosmic.Renderer.Buffer;

import Cosmic.Base;

namespace Cosmic
{

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

        virtual void Bind()   const = 0;
        virtual void Unbind() const = 0;

    public:
        uint32       GetSize()  const { return mSize;  }
        EBufferUsage GetUsage() const { return mUsage; }

    protected:
        uint32 mSize;
        EBufferUsage mUsage;
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