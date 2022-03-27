module;
#include "cspch.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
export module Cosmic.Impl.RendererAPI.OpenGL.OpenGLTestModule;
CS_MODULE_LOG_INFO(Cosmic, Cosmic.Impl.RendererAPI.OpenGL.OpenGLTestModule);

import Cosmic.App.Log;
import Cosmic.App.Module;
import Cosmic.Base;

namespace Cosmic
{

    struct Vertex
    {
        float2 position;
        float3 color;
    };

    export class OpenGLTestModule : public Module
    {
    public:
        void OnInit() override
        {
            Vertex vertices[]
            {
                { { -0.5f,  0.5f }, { 0.2f, 0.57f, 0.87f } },
                { {  0.5f,  0.5f }, { 0.2f, 0.57f, 0.87f } },
                { {  0.5f, -0.5f }, { 0.2f, 0.57f, 0.87f } },
                { { -0.5f, -0.5f }, { 0.2f, 0.57f, 0.87f } },
            };

            uint32 indices[] = { 0, 1, 2, 2, 3, 0 };

            // vertex buffer

            glGenBuffers(1, &mVertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, (const void*)vertices, GL_STATIC_DRAW);

            // index buffer

            glGenBuffers(1, &mIndexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * 6, (const void*)indices, GL_STATIC_DRAW);

            // vertex attributes

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(0 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(2 * sizeof(float)));

            // shaders...

            const char* vertexShaderSource = R"(
            #version 400
            
            layout(location = 0) in vec2 aPosition;
            layout(location = 1) in vec3 aColor;

            out vec4 vColor;

            void main()
            {
                vColor      = vec4(aColor, 1.0);
                gl_Position = vec4(aPosition, 0.0, 1.0);
            })";

            const char* fragmentShaderSource = R"(
            #version 400

            layout(location = 0) out vec4 aFragColor;

            in vec4 vColor;

            void main()
            {
                aFragColor = vColor;
            })";

            GLuint vs = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vs, 1, &vertexShaderSource, NULL);
            glCompileShader(vs);

            GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fs, 1, &fragmentShaderSource, NULL);
            glCompileShader(fs);

            mShaderProgram = glCreateProgram();
            glAttachShader(mShaderProgram, vs);
            glAttachShader(mShaderProgram, fs);
            glLinkProgram(mShaderProgram);
        }

        void OnUpdate() override
        {
            // clearing

            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // binding

            glUseProgram(mShaderProgram);
            glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

            // rendering

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        }

    private:
        GLuint mVertexBuffer;
        GLuint mIndexBuffer;
        GLuint mShaderProgram;
    };

}