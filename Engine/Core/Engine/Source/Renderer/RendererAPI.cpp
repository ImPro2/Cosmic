module;
#include "cspch.hpp"
module Cosmic.Renderer.RendererAPI;

import Cosmic.Impl.RendererAPI.OpenGLRendererAPI;

namespace Cosmic
{

    ERendererAPI PlatformNativeRendererAPI()
    {
#if   defined(CS_PLATFORM_WINDOWS)
        //xreturn ERendererAPI::DirectX;
        return ERendererAPI::OpenGL;
#elif defined(CS_PLATFORM_LINUX) || defined(CS_PLATFORM_ANDROID)
        return ERendererAPI::Vulkan;
#elif defined(CS_PLATFORM_MACOS) || defined(CS_PLATFORM_IOS)
        return ERendererAPI::Metal;
#endif

    }

    Scope<RendererAPI> CreateRendererAPI(ERendererAPI api)
    {
        switch (api)
        {
            case ERendererAPI::OpenGL:  return CreateScope<OpenGLRendererAPI>(api); break;
            case ERendererAPI::DirectX:
            case ERendererAPI::Vulkan:
            case ERendererAPI::Metal:
            default: break;
        }

        //return Scope<nullptr_t>(nullptr);
    }

}