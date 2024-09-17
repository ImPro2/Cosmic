#pragma once
#include "Base/Base.hpp"

namespace Cosmic
{

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

    class Texture2D
    {
    public:
        Texture2D(const String& filePath, ETextureWrapMode wrapMode, ETextureScalingFilter min, ETextureScalingFilter mag);
        Texture2D(uint32 width, uint32 height, ETextureWrapMode wrapMode, ETextureScalingFilter min, ETextureScalingFilter mag);
        virtual ~Texture2D() = default;

    public:
        virtual void SetData(void* data, uint32 size) = 0;
    
    public:
        virtual void Bind(uint32 slot = 0) const = 0;

    public:
        bool   IsLoaded()    const { return mIsLoaded; }
        uint32 GetWidth()    const { return mWidth;    }
        uint32 GetHeight()   const { return mHeight;   }
        String GetFilePath() const { return mFilePath; }
        
        virtual uint32 GetRendererID() const = 0;

    protected:
        bool mIsLoaded;
        uint32 mWidth, mHeight;
        String mFilePath;
    };

    Ref<Texture2D> CreateTexture2D(const String& filePath,      ETextureWrapMode wrapMode = ETextureWrapMode::Repeat, ETextureScalingFilter min = ETextureScalingFilter::Linear, ETextureScalingFilter mag = ETextureScalingFilter::Linear);
    Ref<Texture2D> CreateTexture2D(uint32 width, uint32 height, ETextureWrapMode wrapMode = ETextureWrapMode::Repeat, ETextureScalingFilter min = ETextureScalingFilter::Linear, ETextureScalingFilter mag = ETextureScalingFilter::Linear);

}
