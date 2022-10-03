module;
#include "cspch.hpp"
#include <GLFW/glfw3.h>
export module Cosmic.Impl.RendererAPI.OpenGL.OpenGLGraphicsContext;

import Cosmic.Renderer.GraphicsContext;

namespace Cosmic
{

    export class OpenGLGraphicsContext : public GraphicsContext
    {
    public:
        OpenGLGraphicsContext(GLFWwindow* window);

        void Init()        override;
        void SwapBuffers() override;

    private:
        GLFWwindow* mWindow;
    };

}
