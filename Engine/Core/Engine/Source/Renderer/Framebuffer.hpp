#pragma once
#include "Base/Base.hpp"
#include "Texture.hpp"
#include "Memory/Memory.hpp"

#include <initializer_list>

namespace Cosmic
{

    enum class EFramebufferFlags
    {
        None            = 0,
        SwapchainTarget = 1
    };

    enum class EFramebufferAttachmentType
    {
        ColorAttachment,
        DepthAttachment,
        DepthStencilAttachment
    };

    struct FramebufferAttachmentInfo
    {
        FramebufferAttachmentInfo()                                 = default;
        FramebufferAttachmentInfo(const FramebufferAttachmentInfo&) = default;
        FramebufferAttachmentInfo(ETextureFormat format, EFramebufferAttachmentType type)
            : Format(format), Type(Type)
        {
        }
        
        ETextureFormat             Format = ETextureFormat::RGBA32_SFloat;
        EFramebufferAttachmentType Type   = EFramebufferAttachmentType::ColorAttachment;
    };

    struct FramebufferAttachmentsInfo
    {
        FramebufferAttachmentsInfo()                                  = default;
        FramebufferAttachmentsInfo(const FramebufferAttachmentsInfo&) = default;
        FramebufferAttachmentsInfo(std::initializer_list<FramebufferAttachmentInfo> attachments)
            : Attachments(attachments)
        {
        }

        Vector<FramebufferAttachmentInfo> Attachments;
    };


    struct FramebufferInfo
    {
        FramebufferInfo()                       = default;
        FramebufferInfo(const FramebufferInfo&) = default;

        FramebufferInfo(uint2 size, FramebufferAttachmentsInfo& attachmentsInfo, BitFlags<EFramebufferFlags> flags)
            : Size(size), AttachmentsInfo(attachmentsInfo), Flags(flags)
        {
        }

        FramebufferInfo(uint2 size, FramebufferAttachmentsInfo&& attachmentsInfo, BitFlags<EFramebufferFlags> flags)
            : Size(size), AttachmentsInfo(std::move(attachmentsInfo)), Flags(flags)
        {
        }

        uint2                       Size;
        FramebufferAttachmentsInfo  AttachmentsInfo;
        BitFlags<EFramebufferFlags> Flags;
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
        const Vector<Ref<Texture2D>>& GetAttachments() const { return mAttachments; }

    protected:
        FramebufferInfo        mInfo;
        Vector<Ref<Texture2D>> mAttachments;
    };

    Ref<Framebuffer> CreateFramebuffer(const FramebufferInfo& info);

}
