module;
#include "cspch.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
export module Cosmic.Impl.RendererAPI.OpenGL.OpenGLTestModule;
CS_MODULE_LOG_INFO(Cosmic, Cosmic.Impl.RendererAPI.OpenGL.OpenGLTestModule);

import Cosmic.App.Log;
import Cosmic.App.Module;
import Cosmic.Base;
import Cosmic.App.Application;
import Cosmic.App.IWindow;
import Cosmic.Impl.RendererAPI.OpenGL.OpenGLErrors;
import Cosmic.Impl.RendererAPI.OpenGL.OpenGLShader;
import Cosmic.Renderer.Buffer;
import Cosmic.Renderer.Shader;

//#define GL_CALL(fn) GLClearError(); fn; CS_ASSERT(GLLogCall(), "a")
#define GL_CALL(fn)                                                                                                       \
    Cosmic::OpenGLClearErrors();                                                                                          \
    fn;                                                                                                                   \
    {                                                                                                                     \
        auto errors = Cosmic::OpenGLCheckErrors();                                                                        \
        bool errorOccured = false;                                                                                        \
        for (auto error : errors)                                                                                         \
        {                                                                                                                 \
            CS_LOG_ERROR("OpenGL Error ({}): {} at [{} |:| {}]", error.ErrorCode, error.Description, __FUNCTION__, __LINE__); \
            errorOccured = true;                                                                                          \
        }                                                                                                                 \
        if (errorOccured) CS_BREAK();                                                                                     \
    }                                                                                                                     \

namespace Cosmic
{

    struct Vertex
    {
        float2 position;
        float2 texCoords;
    };

    export class OpenGLTestModule : public Module
    {
    public:
        void OnInit() override
        {
            Vertex vertices[]
            {
                { { -0.5f, -0.5f }, { 0.0f, 0.0f } },
                { {  0.5f, -0.5f }, { 1.0f, 0.0f } },
                { {  0.5f,  0.5f }, { 1.0f, 1.0f } },
                { { -0.5f,  0.5f }, { 0.0f, 1.0f } },
            };

            uint32 indices[] = { 0, 1, 2, 2, 3, 0 };

            mVertexBuffer = CreateVertexBuffer(vertices, sizeof(Vertex) * 4);
            mVertexBuffer->SetLayout({
                VertexBufferElement(EShaderDataType::Float2),
                VertexBufferElement(EShaderDataType::Float2)
            });

            mIndexBuffer = CreateIndexBuffer(indices, 6);

            // textures

            int32 width, height, nChannels;
            uint8* data = stbi_load("C:\\dev\\Cosmic\\Branding\\Logos\\Logo.png", &width, &height, &nChannels, 0);

            GL_CALL(glGenTextures(1, &mTexture));
            GL_CALL(glBindTexture(GL_TEXTURE_2D, mTexture));

            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

            GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
            GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

            stbi_image_free(data);

            // shaders

            mShader = CreateShader("C:/dev/Cosmic/Engine/Core/Engine/Assets/Shaders/Standard2D.glsl");
            mShader->Bind();
        }

        void OnUpdate() override
        {
            // clearing

            GL_CALL(glClearColor(0, 0, 0, 0));
            GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            // updating

            static float3  color          = { 0.5f, 0.5f, 0.5f };
            static float32 colorAddAmount = 0.001f;

            if (color.r >= 1.0f || color.r <= 0.0f)
                colorAddAmount = -colorAddAmount;

            color.r += colorAddAmount;

            GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetHandle());

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                mPosition.x -= 0.001f;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                mPosition.x += 0.001f;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                mPosition.y += 0.001f;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                mPosition.y -= 0.001f;
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
                mCamPos.x -= 0.001f;
            if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
                mCamPos.x += 0.001f;
            if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
                mCamPos.y += 0.001f;
            if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
                mCamPos.y -= 0.001f;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                mRotation -= 1;
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                mRotation += 1;
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                mZoomLevel += 0.001f;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                mZoomLevel -= 0.001f;

            // uniforms

            const float aspectRatio = 800 / 600;

            glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), mPosition);
            transform           = glm::rotate(transform, glm::radians(mRotation), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 view      = glm::translate(glm::mat4(1.0f), mCamPos);
            view                = glm::scale(view, glm::vec3(mZoomLevel, mZoomLevel, 1));
            glm::mat4 viewProj  = proj * view;

            mShader->SetMat4("uViewProjection", viewProj);
            mShader->SetMat4("uTransform", transform);
            mShader->SetFloat3("uColor", color);

            //xOpenGLShader* openGLShader  = static_cast<OpenGLShader*>(mShader.get());
            //xuint32        shaderProgram = openGLShader->GetRendererID();
            //xGL_CALL(GLint vpLocation = glGetUniformLocation(shaderProgram, "uViewProjection"));
            //xGL_CALL(GLint trLocation = glGetUniformLocation(shaderProgram, "uTransform"));
            //xGL_CALL(GLint clLocation = glGetUniformLocation(shaderProgram, "uColor"));
            //xGL_CALL(glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(viewProj)));
            //xGL_CALL(glUniformMatrix4fv(trLocation, 1, GL_FALSE, glm::value_ptr(transform)));
            //xGL_CALL(glUniform3f(clLocation, color.r, color.b, color.g));

            // binding

            mShader->Bind();
            GL_CALL(glBindTexture(GL_TEXTURE_2D, mTexture));
            mVertexBuffer->Bind();
            mIndexBuffer->Bind();

            // rendering

            GL_CALL(glDrawElements(GL_TRIANGLES, mIndexBuffer->GetCount(), GL_UNSIGNED_INT, NULL));
        }

    private:
        Ref<VertexBuffer> mVertexBuffer;
        Ref<IndexBuffer>  mIndexBuffer;
        Ref<Shader>       mShader;
        GLuint            mTexture;
        float32           mZoomLevel = 1.0f;
        glm::vec3         mPosition = { 0.0f, 0.0f, 0.0f };
        glm::vec3         mCamPos   = { 0.0f, 0.0f, 0.0f };
        float32           mRotation = 0.0f;
    };

}