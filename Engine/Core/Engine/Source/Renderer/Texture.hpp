#pragma once
#include "Base/Base.hpp"
#include "Memory/Memory.hpp"

namespace Cosmic
{

    enum class ETextureFormat : int16
    {
        Unknown = -1,

        R8_UNorm,    R8_SNorm,    R8_UInt,    R8_SInt,    R8_SRGB,
        RG8_UNorm,   RG8_SNorm,   RG8_UInt,   RG8_SInt,   RG8_SRGB,
        RGB8_UNorm,  RGB8_SNorm,  RGB8_UInt,  RGB8_SInt,  RGB8_SRGB,
        BGR8_UNorm,  BGR8_SNorm,  BGR8_UInt,  BGR8_SInt,  BGR8_SRGB,
        RGBA8_UNorm, RGBA8_SNorm, RGB8A_UInt, RGBA8_SInt, RGBA8_SRGB,
        BGRA8_UNorm, BGRA8_SNorm, BGRA8_UInt, BGRA8_SInt, BGRA8_SRGB,

        R16_UNorm,    R16_SNorm,    R16_UInt,    R16_SInt,    R16_SFloat,
        RG16_UNorm,   RG16_SNorm,   RG16_UInt,   RG16_SInt,   RG16_SFloat,
        RGB16_UNorm,  RGB16_SNorm,  RGB16_UInt,  RGB16_SInt,  RGB16_SFloat,
        RGBA16_UNorm, RGBA16_SNorm, RGB16A_UInt, RGBA16_SInt, RGBA16_SFloat,

        R32_UInt,    R32_SInt,    R32_SFloat,
        RG32_UInt,   RG32_SInt,   RG32_SFloat,
        RGB32_UInt,  RGB32_SInt,  RGB32_SFloat,
        RGB32A_UInt, RGBA32_SInt, RGBA32_SFloat,

        Depth16_UNorm, Depth32_SFloat,
        Depth16_UNorm_Stencil8_UInt, Depth24_UNorm_Stencil8_UInt, Depth32_SFloat_Stencil8_UInt
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
            uint2                 size   = { 0, 0 }, // If it's loaded from a file.
            ETextureWrapMode      wrap   = ETextureWrapMode::ClampToBorder,
            ETextureScalingFilter min    = ETextureScalingFilter::Linear,
            ETextureScalingFilter mag    = ETextureScalingFilter::Linear,
            ETextureFormat        format = ETextureFormat::RGBA32_SFloat
        )
            : Size(size), WrapMode(wrap), MinScalingFilter(min), MagScalingFilter(mag), Format(format)
        {
        }

        uint2                 Size;
        ETextureWrapMode      WrapMode;
        ETextureScalingFilter MinScalingFilter;
        ETextureScalingFilter MagScalingFilter;
        ETextureFormat        Format;
    };

    class Texture2D : public IRefCounted
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
        uint2 GetSize() const { return mInfo.Size;  }
        
        virtual void* GetNativeHandle() const = 0;

    protected:
        String mFilePath;
        Texture2DInfo mInfo;
    };

    Ref<Texture2D> CreateTexture2D(const String& filePath, const Texture2DInfo& info);
    Ref<Texture2D> CreateTexture2D(const Texture2DInfo& info);

}
