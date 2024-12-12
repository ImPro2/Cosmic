#pragma once
#include "Base/Base.hpp"
#include "Texture.hpp"
#include "Memory/SmartPtrs.hpp"

#include <initializer_list>

namespace Cosmic
{

    struct FramebufferTextureInfo
    {
        FramebufferTextureInfo(const FramebufferTextureInfo&) = default;

        FramebufferTextureInfo(
            ETextureFormat        format = ETextureFormat::RGBA8_Float,
            ETextureWrapMode      wrap   = ETextureWrapMode::ClampToBorder,
            ETextureScalingFilter min    = ETextureScalingFilter::Linear,
            ETextureScalingFilter mag    = ETextureScalingFilter::Linear
        )
            : WrapMode(wrap), MinScalingFilter(min), MagScalingFilter(mag), Format(format)
        {
        }
        
        ETextureFormat        Format;
        ETextureWrapMode      WrapMode;
        ETextureScalingFilter MinScalingFilter;
        ETextureScalingFilter MagScalingFilter;
    };

    struct FramebufferAttachmentsInfo
    {
        FramebufferAttachmentsInfo()                                  = default;
        FramebufferAttachmentsInfo(const FramebufferAttachmentsInfo&) = default;
        FramebufferAttachmentsInfo(std::initializer_list<FramebufferTextureInfo> attachments)
            : Attachments(attachments)
        {
        }

        Vector<FramebufferTextureInfo> Attachments;
    };

    struct FramebufferInfo
    {
        FramebufferInfo()                       = default;
        FramebufferInfo(const FramebufferInfo&) = default;

        FramebufferInfo(
            const FramebufferAttachmentsInfo& attachmentsInfo,
            uint32                            width,
            uint32                            height,
            uint32                            samples         = 1,
            bool                              swapchainTarget = false
        )
            : AttachmentsInfo(attachmentsInfo), Width(width), Height(height), Samples(samples), SwapChainTarget(swapchainTarget)
        {
        }

        FramebufferInfo(
            FramebufferAttachmentsInfo&& attachmentsInfo,
            uint32                       width,
            uint32                       height,
            uint32                       samples         = 1,
            bool                         swapchainTarget = false
        )
            : AttachmentsInfo(std::move(attachmentsInfo)), Width(width), Height(height), Samples(samples), SwapChainTarget(swapchainTarget)
        {
        }

        FramebufferAttachmentsInfo AttachmentsInfo;

        uint32 Width;
        uint32 Height;
        uint32 Samples = 1;
        bool   SwapChainTarget = false;

    };

    class Framebuffer : public IRefCounted
    {
    public:
        Framebuffer(const FramebufferInfo& info);

    public:
        const FramebufferInfo& GetInfo() const { return mInfo; }

    public:
        virtual void Invalidate() = 0;
        virtual void Resize(uint32 width, uint32 height) = 0;
        virtual void ClearAttachment(uint32 attachmentIndex, int32 value) = 0;

        virtual int32 ReadPixel(uint32 attachmentIndex, int2 pos) = 0;

    public:
        virtual void Bind()   = 0;
        virtual void Unbind() = 0;

    public:
        virtual uint32 GetColorAttachmentRendererID() = 0;

    protected:
        FramebufferInfo        mInfo;
        Vector<Ref<Texture2D>> mTextures;
    };

    Ref<Framebuffer> CreateFramebuffer(const FramebufferInfo& info);

}
