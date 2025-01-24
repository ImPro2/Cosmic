#include "cspch.hpp"
#include "Shader.hpp"
#include "App/File.hpp"

#include "Renderer/RendererAPI.hpp"
#include "Impl/RendererAPI/OpenGL/OpenGLShader.hpp"
//#include "Impl/RendererAPI/Vulkan/VulkanShader.hpp"

CS_MODULE_LOG_INFO(Cosmic, Renderer.Shader);

namespace Cosmic
{

    Shader::Shader(const ShaderInfo& info)
        : mInfo(info)
    {
        String extension = File(info.FilePath).GetExtension();

        if (extension == ".spv")
        {
            mBinary.Binary = File(info.FilePath).ReadBinary();
        }
        else if (extension == ".glsl")
        {
            String src = File(info.FilePath).Read();

            ShaderSourceMap srcMap = ShaderCompiler::PreProcess(src);
            mBinary = ShaderCompiler::CompileToSpirv(mInfo.Name, EShaderSourceLanguage::GLSL, srcMap);

            // TODO: cache spv
        }
        else
        {
            CS_ASSERT(false, "Unsupported shader extension");
        }

        mReflectionData = ShaderCompiler::Reflect(mBinary);
    }

    Ref<Shader> CreateShader(const ShaderInfo& info)
    {
        switch (RendererAPI::Get())
        {
            //case ERendererAPI::OpenGL: return CreateRef<OpenGLShader>(info);
            //case ERendererAPI::Vulkan: return CreateRef<VulkanShader>(info);
        }
        return {};
    }

}
