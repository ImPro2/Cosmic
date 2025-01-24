#include "cspch.hpp"
#include "Texture.hpp"

#include "Renderer/RendererAPI.hpp"
#include "Impl/RendererAPI/OpenGL/OpenGLTexture.hpp"

namespace Cosmic
{

    Texture2D::Texture2D(const String& filePath, const Texture2DInfo& info)
        : mFilePath(filePath), mInfo(info)
    {
        CS_PROFILE_FN();
    }

    Texture2D::Texture2D(const Texture2DInfo& info)
        : mInfo(info)
    {
        CS_PROFILE_FN();
    }

    Ref<Texture2D> CreateTexture2D(const String& filePath, const Texture2DInfo& info)
    {
        CS_PROFILE_FN();

        switch (RendererAPI::Get())
        {
            //case ERendererAPI::OpenGL: return CreateRef<OpenGLTexture2D>(filePath, info);
        }
        return {};
    }

    Ref<Texture2D> CreateTexture2D(const Texture2DInfo& info)
    {
        CS_PROFILE_FN();

        switch (RendererAPI::Get())
        {
            //case ERendererAPI::OpenGL: return CreateRef<OpenGLTexture2D>(info);
        }

        return {};
    }

}
