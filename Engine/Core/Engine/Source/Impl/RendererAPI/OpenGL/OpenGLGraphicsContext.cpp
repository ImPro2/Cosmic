#include "cspch.hpp"
#include "OpenGLGraphicsContext.hpp"
#include "OpenGLErrors.hpp"

#include "glad/glad.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

CS_MODULE_LOG_INFO(Cosmic, Impl.RendererAPI.OpenGL.OpenGLGraphicsContext);

#include "Base/Base.hpp"
#include "App/Log/Log.hpp"
#include "App/Module.hpp"

namespace Cosmic
{

    namespace Utils
    {

        const char* OpenGLSourceToStr(GLenum source)
        {
            switch (source)
            {
                case GL_DEBUG_SOURCE_API:             return "API";
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Window System";
                case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
                case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Third Party";
                case GL_DEBUG_SOURCE_APPLICATION:     return "Application";
                case GL_DEBUG_SOURCE_OTHER:           return "Other";
            }

            return "Unknown";
        }

        const char* OpenGLErrorTypeToStr(GLenum type)
        {
            switch (type)
            {
                case GL_DEBUG_TYPE_ERROR:               return "Error";
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behavior";
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behavior";
                case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
                case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
                case GL_DEBUG_TYPE_MARKER:              return "Marker";
                case GL_DEBUG_TYPE_PUSH_GROUP:          return "Push Group";
                case GL_DEBUG_TYPE_POP_GROUP:           return "Pop Group";
                case GL_DEBUG_TYPE_OTHER:               return "Other";
            }

            return "Unknown";
        }

        const char* OpenGLSeverityToStr(GLenum severity)
        {
            switch (severity)
            {
                case GL_DEBUG_SEVERITY_LOW:          return "Low";
                case GL_DEBUG_SEVERITY_MEDIUM:       return "Medium";
                case GL_DEBUG_SEVERITY_HIGH:         return "High";
                case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
            }

            return "Unknown";
        }

    }

    static APIENTRY void OpenGLDebugMessageCallback(
        GLenum source,
        GLenum type,
        GLenum id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam
    )
    {
        const char* sourceStr   = Utils::OpenGLSourceToStr(source);
        const char* typeStr     = Utils::OpenGLErrorTypeToStr(type);
        const char* severityStr = Utils::OpenGLSeverityToStr(severity);
        
        CS_LOG_ERROR("OpenGL Error (Source: {}, Type: {}, Severity: {}): {}", sourceStr, typeStr, severityStr, message);
        CS_BREAK();
    }

    OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* window)
        : mWindow(window)
    {
    }

    void OpenGLGraphicsContext::Init()
    {
        CS_PROFILE_FN();

        glfwMakeContextCurrent(mWindow);
        int32 status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        CS_ASSERT(status, "Failed to initialize Glad!");

        glDebugMessageCallback(OpenGLDebugMessageCallback, nullptr);

        mInfo.Vendor   = (const char*)glGetString(GL_VENDOR);
        mInfo.Renderer = (const char*)glGetString(GL_RENDERER);
        mInfo.Version  = (const char*)glGetString(GL_VERSION);

        CS_LOG_DEBUG("OpenGL Info:");
        CS_LOG_DEBUG("    Vendor:   {}", mInfo.Vendor);
        CS_LOG_DEBUG("    Renderer: {}", mInfo.Renderer);
        CS_LOG_DEBUG("    Version:  {}", mInfo.Version);

        //xModuleSystem::Add<OpenGLTestModule>();
    }

    void OpenGLGraphicsContext::SwapBuffers()
    {
        CS_PROFILE_FN();

        glfwSwapBuffers(mWindow);
    }

}
