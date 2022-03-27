module;
#include "cspch.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
module Cosmic.Impl.RendererAPI.OpenGL.OpenGLGraphicsContext;
CS_MODULE_LOG_INFO(Cosmic, Cosmic.Impl.RendererAPI.OpenGL.OpenGLGraphicsContext);

import Cosmic.Base;
import Cosmic.App.Log;
import Cosmic.App.Module;
import Cosmic.Impl.RendererAPI.OpenGL.OpenGLTestModule;

namespace Cosmic
{

    OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* window)
        : mWindow(window)
    {
    }

    void OpenGLGraphicsContext::Init()
    {
        glfwMakeContextCurrent(mWindow);
        int32 status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        CS_ASSERT(status, "Failed to initialize Glad!");

        mInfo.Vendor   = (const char*)glGetString(GL_VENDOR);
        mInfo.Renderer = (const char*)glGetString(GL_RENDERER);
        mInfo.Version  = (const char*)glGetString(GL_VERSION);

        CS_LOG_DEBUG("OpenGL Info:");
        CS_LOG_DEBUG("    Vendor:   {}", mInfo.Vendor);
        CS_LOG_DEBUG("    Renderer: {}", mInfo.Renderer);
        CS_LOG_DEBUG("    Version:  {}", mInfo.Version);

        ModuleSystem::Add<OpenGLTestModule>();
    }

    void OpenGLGraphicsContext::SwapBuffers()
    {
        glfwSwapBuffers(mWindow);
    }

}