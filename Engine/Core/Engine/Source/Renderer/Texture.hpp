#pragma once
#include "Base/Base.hpp"

namespace Cosmic
{

    enum class ETextureFormat
    {
        R8_Float,     R8_UInt,     R8_SInt,
        R16_Float,    R16_UInt,    R16_SInt,
        R32_Float,    R32_UInt,    R32_SInt,

        RG8_Float,    RG8_UInt,    RG8_SInt,
        RG16_Float,   RG16_UInt,   RG16_SInt,
        RG32_Float,   RG32_UInt,   RG32_SInt,

        RGB8_Float,   RGB8_UInt,   RGB8_SInt,
        RGB16_Float,  RGB16_UInt,  RGB16_SInt,
        RGB32_Float,  RGB32_UInt,  RGB32_SInt,

        RGBA8_Float,  RGBA8_UInt,  RGBA8_SInt,
        RGBA16_Float, RGBA16_UInt, RGBA16_SInt,
        RGBA32_Float, RGBA32_UInt, RGBA32_SInt,

        Depth16, Depth24, Depth32,
        Depth24Stencil8, Depth32Stencil8
    };

    enum class ETextureAttachmentType
    {
        ColorAttachment,
        DepthAttachment,
        DepthStencilAttachment
    };

    enum class ETextureWrapMode
    {
        ClampToEdge,
        ClampToBorder,
        MirroredRepeat,
        Repeat
    };

    enum class ETextureScalingFilter
    {
        Nearest,
        Linear,
        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LineaerMipmapLinear
    };

    struct Texture2DInfo
    {
        Texture2DInfo()                     = default;
        Texture2DInfo(const Texture2DInfo&) = default;
        Texture2DInfo(
            uint32                width  = 0, // If it's loaded from a file.
            uint32                height = 0,
            ETextureWrapMode      wrap   = ETextureWrapMode::ClampToBorder,
            ETextureScalingFilter min    = ETextureScalingFilter::Linear,
            ETextureScalingFilter mag    = ETextureScalingFilter::Linear,
            ETextureFormat        format = ETextureFormat::RGBA8_Float
        )
            : Width(width), Height(height), WrapMode(wrap), MinScalingFilter(min), MagScalingFilter(mag), Format(format)
        {
        }

        uint32                Width;
        uint32                Height;
        ETextureWrapMode      WrapMode;
        ETextureScalingFilter MinScalingFilter;
        ETextureScalingFilter MagScalingFilter;
        ETextureFormat        Format;
    };

    class Texture2D
    {
    public:
        Texture2D(const String& filePath, const Texture2DInfo& info);
        Texture2D(const Texture2DInfo& info);
        virtual ~Texture2D() = default;

    public:
        virtual void SetData(void* data, uint32 size) = 0;
    
    public:
        virtual void Bind(uint32 slot = 0) const = 0;

    public:
        const Texture2DInfo& GetInfo() { return mInfo; }

        uint32 GetWidth()  const { return mInfo.Width;  }
        uint32 GetHeight() const { return mInfo.Height; }
        
        virtual uint32 GetRendererID() const = 0;
        virtual ETextureAttachmentType GetAttachmentType() const = 0;

    protected:
        String mFilePath;
        Texture2DInfo mInfo;
    };

    Ref<Texture2D> CreateTexture2D(const String& filePath, const Texture2DInfo& info);
    Ref<Texture2D> CreateTexture2D(const Texture2DInfo& info);

}
