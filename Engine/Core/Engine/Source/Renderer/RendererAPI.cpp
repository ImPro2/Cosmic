#include "cspch.hpp"
#include "RendererAPI.hpp"

#include "Impl/RendererAPI/OpenGL/OpenGLRendererAPI.hpp"

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
        return ERendererAPI::OpenGL;
#elif defined(CS_PLATFORM_MACOS) || defined(CS_PLATFORM_IOS)
        return ERendererAPI::Metal;
#endif

    }

    Ref<RendererAPI> CreateRendererAPI(ERendererAPI api)
    {
        CS_PROFILE_FN();

        switch (api)
        {
			case ERendererAPI::OpenGL:  return CreateRef<OpenGLRendererAPI>(api); break;
            case ERendererAPI::DirectX:
            case ERendererAPI::Vulkan:
            case ERendererAPI::Metal:
            default: break;
        }

        //return Scope<nullptr_t>(nullptr);
    }

}
