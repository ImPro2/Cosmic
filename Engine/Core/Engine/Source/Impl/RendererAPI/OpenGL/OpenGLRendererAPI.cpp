module;
#include "cspch.hpp"
#include <glad/glad.h>
module Cosmic.Impl.RendererAPI.OpenGLRendererAPI;
CS_MODULE_LOG_INFO(Sandbox, SandboxApp);

namespace Cosmic
{

    OpenGLRendererAPI::OpenGLRendererAPI(ERendererAPI api)
        : RendererAPI(api)
    {
        CS_PROFILE_FN();
    }

    OpenGLRendererAPI::~OpenGLRendererAPI()
    {
        CS_PROFILE_FN();
    }

    void OpenGLRendererAPI::Init()
    {
        CS_PROFILE_FN();
    }

    void OpenGLRendererAPI::Shutdown()
    {
        CS_PROFILE_FN();
    }

    void OpenGLRendererAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
    {
        CS_PROFILE_FN();

        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(float4 color)
    {
        CS_PROFILE_FN();

        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        CS_PROFILE_FN();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::Render(EPrimitiveTopology primitiveTopology, uint32 count)
    {
        CS_PROFILE_FN();

        switch (primitiveTopology)
        {
            case EPrimitiveTopology::PointList:       glDrawArrays  (GL_POINTS,         0, count);                     return;
            case EPrimitiveTopology::PointIndexed:    glDrawElements(GL_POINTS,     count, GL_UNSIGNED_INT, (void*)0); return;
            case EPrimitiveTopology::LineList:        glDrawArrays  (GL_LINES,          0, count);                     return;
            case EPrimitiveTopology::LineIndexed:     glDrawElements(GL_LINES,      count, GL_UNSIGNED_INT, (void*)0); return;
            case EPrimitiveTopology::LineStrip:       glDrawArrays  (GL_LINE_STRIP,     0, count);                     return;
            case EPrimitiveTopology::TriangleList:    glDrawArrays  (GL_TRIANGLES,      0, count);                     return;
            case EPrimitiveTopology::TriangleIndexed: glDrawElements(GL_TRIANGLES,  count, GL_UNSIGNED_INT, (void*)0); return;
            case EPrimitiveTopology::TriangleStrip:   glDrawArrays  (GL_TRIANGLE_STRIP, 0, count);                     return;
        }
    }

}
