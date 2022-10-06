module;
#include "cspch.hpp"
export module Cosmic.Renderer.Framebuffer;

import Cosmic.Base;

namespace Cosmic
{

    export struct FramebufferInfo
    {
        uint32 Width;
        uint32 Height;
        uint32 Samples = 1;
        bool   SwapChainTarget = false;
    };

    export class Framebuffer
    {
    public:
        Framebuffer(const FramebufferInfo& info);

    public:
        const FramebufferInfo& GetInfo() const { return mInfo; }

    public:
        virtual void Invalidate() = 0;
        virtual void Resize(uint32 width, uint32 height) = 0;

    public:
        virtual void Bind()   = 0;
        virtual void Unbind() = 0;

    public:
        virtual uint32 GetColorAttachmentRendererID() = 0;

    protected:
        FramebufferInfo mInfo;
    };

    export Ref<Framebuffer> CreateFramebuffer(const FramebufferInfo& info);

}