module;
#include "cspch.hpp"
#include <glad/glad.h>
module Cosmic.Impl.RendererAPI.OpenGLRendererAPI;
CS_MODULE_LOG_INFO(Sandbox, SandboxApp);

namespace Cosmic
{

    void OpenGLRendererAPI::Init()
    {

    }

    void OpenGLRendererAPI::Shutdown()
    {

    }

    void OpenGLRendererAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(float4 color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::Render(EPrimitiveTopology primitiveTopology, uint32 count)
    {
        switch (primitiveTopology)
        {
            PointList:       glDrawArrays  (GL_POINTS,         0, count);                     break;
            PointIndexed:    glDrawElements(GL_POINTS,     count, GL_UNSIGNED_INT, (void*)0); break;
            LineList:        glDrawArrays  (GL_LINES,          0, count);                     break;
            LineIndexed:     glDrawElements(GL_LINES,      count, GL_UNSIGNED_INT, (void*)0); break;
            LineStrip:       glDrawArrays  (GL_LINE_STRIP,     0, count);                     break;
            TriangleList:    glDrawArrays  (GL_TRIANGLES,      0, count);                     break;
            TriangleIndexed: glDrawElements(GL_TRIANGLES,  count, GL_UNSIGNED_INT, (void*)0); break;
            TriangleStrip:   glDrawArrays  (GL_TRIANGLE_STRIP, 0, count);                     break;
        }
    }

}