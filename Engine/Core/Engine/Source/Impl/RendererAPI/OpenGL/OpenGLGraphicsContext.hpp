#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Renderer/GraphicsContext.hpp"

namespace Cosmic
{

    class OpenGLGraphicsContext : public GraphicsContext
    {
    public:
        OpenGLGraphicsContext(GLFWwindow* window);

        void Init()        override;
        void SwapBuffers() override;

    private:
        GLFWwindow* mWindow;
    };

}
