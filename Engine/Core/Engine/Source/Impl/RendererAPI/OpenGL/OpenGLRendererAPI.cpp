#include "cspch.hpp"
#include "OpenGLRendererAPI.hpp"
#include "OpenGLErrors.hpp"

#include <glad/glad.h>

CS_MODULE_LOG_INFO(Cosmic, Impl.RendererAPI.OpenGL.OpenGLRendererAPI);

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

        GL_CALL(glViewport(x, y, width, height));
    }

    void OpenGLRendererAPI::SetClearColor(float4 color)
    {
        CS_PROFILE_FN();

        GL_CALL(glClearColor(color.r, color.g, color.b, color.a));
    }

    void OpenGLRendererAPI::Clear()
    {
        CS_PROFILE_FN();

        GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void OpenGLRendererAPI::Render(EPrimitiveTopology primitiveTopology, uint32 count)
    {
        CS_PROFILE_FN();

        switch (primitiveTopology)
        {
            case EPrimitiveTopology::PointList:       GL_CALL(glDrawArrays  (GL_POINTS,         0, count));                     return;
            case EPrimitiveTopology::PointIndexed:    GL_CALL(glDrawElements(GL_POINTS,     count, GL_UNSIGNED_INT, (void*)0)); return;
            case EPrimitiveTopology::LineList:        GL_CALL(glDrawArrays  (GL_LINES,          0, count));                     return;
            case EPrimitiveTopology::LineIndexed:     GL_CALL(glDrawElements(GL_LINES,      count, GL_UNSIGNED_INT, (void*)0)); return;
            case EPrimitiveTopology::LineStrip:       GL_CALL(glDrawArrays  (GL_LINE_STRIP,     0, count));                     return;
            case EPrimitiveTopology::TriangleList:    GL_CALL(glDrawArrays  (GL_TRIANGLES,      0, count));                     return;
            case EPrimitiveTopology::TriangleIndexed: GL_CALL(glDrawElements(GL_TRIANGLES,  count, GL_UNSIGNED_INT, (void*)0)); return;
            case EPrimitiveTopology::TriangleStrip:   GL_CALL(glDrawArrays  (GL_TRIANGLE_STRIP, 0, count));                     return;
        }
    }

}
