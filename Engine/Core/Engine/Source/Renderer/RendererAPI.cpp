module;
#include "cspch.hpp"
module Cosmic.Renderer.RendererAPI;

import Cosmic.Impl.RendererAPI.OpenGLRendererAPI;

namespace Cosmic
{

    RendererAPI::RendererAPI(ERendererAPI api)
    {
        CS_PROFILE_FN();

        sAPI = api;
    }

    RendererAPI::~RendererAPI()
    {
        CS_PROFILE_FN();

    }

    ERendererAPI PlatformNativeRendererAPI()
    {
        CS_PROFILE_FN();

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
        CS_PROFILE_FN();

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
